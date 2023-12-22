// @ DimmaK, Kyiv, Ukraine, Camera controller project december 2023.

//? Source code
#include "DroidPawnBase.h"

//? C++ std

//? UnrealEngine
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

//? Project headers
#include "AdBlockerController.h"
#include "CameraAdBlockerComponent.h"
#include "PawnMovingComponent.h"
#include "PawnSteeringComponent.h"

ADroidPawnBase::ADroidPawnBase()
{
	//? Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MassOrigin = CreateDefaultSubobject<UBoxComponent>(TEXT("Mass Plate"));
	RootComponent = MassOrigin;	   //? placing collider into the root

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	BaseMesh->SetupAttachment(RootComponent);	 //* to root

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret"));
	TurretMesh->SetupAttachment(BaseMesh);

	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Turret Nozzle"));
	SpawnPoint->SetupAttachment(TurretMesh);

	ViewClearance = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Camera Blocker"));
	ViewClearance->SetupAttachment(RootComponent);	  //* to root

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraHolder"));
	SpringArm->SetupAttachment(RootComponent);	  //* to root

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	//? Turn ON tanks forward engine
	EngineComponent = CreateDefaultSubobject<UPawnMovingComponent>(TEXT("Moving physics"));
	EngineComponent->SetRoot(Cast<UPrimitiveComponent>(RootComponent));

	//? Turn ON tanks wheels rotator
	SteerComponent = CreateDefaultSubobject<UPawnSteeringComponent>(TEXT("Steering physics"));
	SteerComponent->SetRoot(Cast<UPrimitiveComponent>(RootComponent));

	AdBlocker = CreateDefaultSubobject<UCameraAdBlockerComponent>(TEXT("Anti-Blockator for camera"));
}

void ADroidPawnBase::BeginPlay()
{
	Super::BeginPlay();

	//? Make camera anti blockator available
	AdBlocker->Launch(SpawnPoint, ViewClearance, Camera);
}

void ADroidPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateTurret();
}

void ADroidPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	AAdBlockerController* PC = Cast<AAdBlockerController>(Controller);

	if (EIC == nullptr || PC == nullptr) return;

	EIC->BindAction(PC->MovePawnAction, ETriggerEvent::Triggered, this,
					&ADroidPawnBase::Move);	   //? +1.0, -1.0 on pressed
	EIC->BindAction(PC->MovePawnAction, ETriggerEvent::Completed, this, &ADroidPawnBase::Move);	   //? 0.0 on released

	EIC->BindAction(PC->SteerPawnAction, ETriggerEvent::Triggered, this,
					&ADroidPawnBase::Steer);	//? +1.0, -1.0 on pressed
	EIC->BindAction(PC->SteerPawnAction, ETriggerEvent::Completed, this, &ADroidPawnBase::Steer);	 //? 0.0 on released

	EIC->BindAction(PC->RotateCameraAction, ETriggerEvent::Triggered, this, &ADroidPawnBase::RotateCamera);
	EIC->BindAction(PC->RotateCameraAction, ETriggerEvent::Completed, this, &ADroidPawnBase::RotateCamera);

	ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();

	if (LocalPlayer == nullptr) return;

	UEnhancedInputLocalPlayerSubsystem* SubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (SubSystem == nullptr) return;

	SubSystem->ClearAllMappings();
	SubSystem->AddMappingContext(PC->PawnMappingContext, 0);
}

void ADroidPawnBase::RotateTurret()
{
	float SpringArmCurrentYaw = (float)SpringArm->GetRelativeRotation().Yaw;

	FRotator TurretRotation = TurretMesh->GetRelativeRotation();

	//? We want to rotate turret in 360 deg without clipping
	if (std::abs(TurretRotation.Yaw - SpringArmCurrentYaw) > 240.f)
	{
		if (TurretRotation.Yaw > 0.f)
		{
			TurretRotation.Yaw -= 360.f;
		}
		else
		{
			TurretRotation.Yaw += 360.f;
		}
	}

	//? Smooth Yaw
	TurretRotation.Yaw = FMath::FInterpTo(				 //
		TurretRotation.Yaw,								 //
		SpringArmCurrentYaw,							 //
		UGameplayStatics::GetWorldDeltaSeconds(this),	 //
		TurretRotationSpeed								 //
	);

	//? Finally rotate
	TurretMesh->SetRelativeRotation(TurretRotation);
}

void ADroidPawnBase::Move(const FInputActionValue& ActionValue)
{
	auto Input = ActionValue.Get<FInputActionValue::Axis1D>();
	EngineComponent->IncrementSpeed(Input);
}

void ADroidPawnBase::Steer(const FInputActionValue& ActionValue)
{
	auto Input = ActionValue.Get<FInputActionValue::Axis1D>();
	SteerComponent->IncrementSpeed(Input);
}

void ADroidPawnBase::RotateCamera(const FInputActionValue& ActionValue)
{
	//? FRotator(Pitch, Yaw, Roll)
	FRotator Input(ActionValue[0], ActionValue[1], 0);

	float DeltaSeconds = GetWorld()->GetDeltaSeconds();

	FRotator CurrentRotation = SpringArm->GetRelativeRotation();
	FRotator NewRotation = CurrentRotation;

	//? Prevent lag when frames different in time
	Input *= DeltaSeconds * RotateScale;

	NewRotation.Yaw += Input.Yaw;
	NewRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, NewRotation.Yaw, DeltaSeconds, CameraRotationSpeed);

	NewRotation.Pitch += Input.Pitch;
	NewRotation.Pitch = FMath::ClampAngle(NewRotation.Pitch, CameraMinimumPitch, CameraMaximumPitch);
	NewRotation.Pitch = FMath::FInterpTo(CurrentRotation.Pitch, NewRotation.Pitch, DeltaSeconds, CameraRotationSpeed);

	NewRotation.Roll = 0;

	SpringArm->SetRelativeRotation(NewRotation);
}
