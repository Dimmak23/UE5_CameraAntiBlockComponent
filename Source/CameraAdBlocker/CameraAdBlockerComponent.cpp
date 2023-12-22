// @ DimmaK, Kyiv, Ukraine, Camera controller project december 2023.

//? Source code
#include "CameraAdBlockerComponent.h"

//? C++ std

//? UnrealEngine
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"

//? Sets default values for this component's properties
UCameraAdBlockerComponent::UCameraAdBlockerComponent()
{
	//? Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
	//? features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//? In case MinOpacity entered greater then MaxOpacity
	MinOpacity = std::clamp(MinOpacity, 0.01f, MaxOpacity);
}

//? Called when the game starts
void UCameraAdBlockerComponent::BeginPlay() { Super::BeginPlay(); }

//? Called every frame
void UCameraAdBlockerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
											  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//? Check maybe should make transparent something
	if (bInit && Enabled) CheckOccludedMeshes();
}

void UCameraAdBlockerComponent::Launch(USceneComponent* Target, UCapsuleComponent* Capsule, UCameraComponent* Camera)
{
	ViewTarget = Target;
	ViewClearance = Capsule;
	ViewCamera = Camera;

	//? If all of them valid we can tick
	if (ViewTarget && ViewClearance && ViewCamera)
	{
		bInit = true;
	}
}

void UCameraAdBlockerComponent::CheckOccludedMeshes()
{
	//? Prepare all static meshes to shows up again
	if (BlockingMeshes.Num())
	{
		for (auto& Component : BlockingMeshes)
		{
			if (Component.Value.bToModify) Component.Value.bToModify = false;
		}
	}

	FVector Start = ViewCamera->GetComponentLocation();
	FVector End = ViewTarget->GetComponentLocation() + ViewTarget->GetForwardVector() * ViewDistance;

	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;
	CollisionObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> OutHits;

	auto ShouldDebug = bDebugLineTraces ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;

	bool bGotHits = UKismetSystemLibrary::CapsuleTraceMultiForObjects(
		//? World context
		GetWorld(),
		//? Start of line segment.
		Start,
		//? End of line segment.
		End,
		//? Radius of the capsule to sweep
		ViewClearance->GetScaledCapsuleRadius() * CapsulePercentageForTrace,
		//? Distance from center of capsule to tip of hemisphere endcap.
		ViewClearance->GetScaledCapsuleHalfHeight() * CapsulePercentageForTrace,
		//? Array of Object Types to trace
		CollisionObjectTypes,
		//? True to test against complex collision,
		//? false to test against simplified collision.
		true,
		//? Who we will be ignore
		ActorsToIgnore,
		//? Draw debug type
		ShouldDebug,
		//? A list of hits, sorted along the trace from start to finish.
		//? The blocking hit will be the last hit, if there was one.
		OutHits,
		//? Ignore self
		false,
		//? TraceColor
		FLinearColor::Green,
		//? TraceHitColor
		FLinearColor::Red	 //
	);

	if (bGotHits)
	{
		for (auto& OutHit : OutHits)
		{
			//? We need only 'this' pawn to hide meshes from
			//* But of course this is redudant considering that 'CollisionTest' is enabled
			if (OutHit.GetActor() != GetOwner()) continue;

			UStaticMeshComponent* MeshHitted = Cast<UStaticMeshComponent>(OutHit.GetComponent());
			if (!MeshHitted) continue;	  //? It's not a Mesh, go further

			//? Check if static mesh is in the array already
			FCameraBlocker* ExistingMesh = BlockingMeshes.Find(MeshHitted);
			if (ExistingMesh)
			{
				ExistingMesh->bToModify = true;	   //? Continue to hide this static mesh
			}
			else
			{
				//? Create new
				FCameraBlocker MeshToHideInstance;
				MeshToHideInstance.StaticMesh = MeshHitted;
				MeshToHideInstance.bToModify = true;	//? And hide
				MeshToHideInstance.Materials = MeshHitted->GetMaterials();

				//? Add new static mesh to map, and hide it
				BlockingMeshes.Add(MeshHitted, MeshToHideInstance);
			}
		}
	}

	CalculateDistanceToTarget();

	OnShowHide();
}

void UCameraAdBlockerComponent::OnShowHide()
{
	if (!BlockingMeshes.Num()) return;

	CalculateNewOpacity();

	UMaterialInstanceDynamic* _MaterialDynamic{ nullptr };

	//! For stable work you have to create new dynamic material very time you need it
	if (FadeMaterial && (NewOpacity < MaxOpacity))
	{
		//? Createting dynamic material, we can assign some dynamic values
		_MaterialDynamic = UMaterialInstanceDynamic::Create(FadeMaterial, Cast<AActor>(this));
		//? Set the parameters for Dynamic Material Instance,
		//? these will be the names inside of the Material/MaterialInstance
		_MaterialDynamic->SetScalarParameterValue(TEXT("Opacity"), NewOpacity);
	}

	for (auto& Component : BlockingMeshes)
	{
		if (Component.Value.bToModify && (NewOpacity < MaxOpacity))
		{
			//? Overwrite materials for specific mesh to dynamic
			for (int matIdx{}; matIdx < Component.Value.Materials.Num(); ++matIdx)
			{
				if (_MaterialDynamic)
				{
					Component.Value.StaticMesh->SetMaterial(matIdx, _MaterialDynamic);
				}
			}
		}
		else
		{
			//? Change back materials for specific mesh to originals
			for (int matIdx{}; matIdx < Component.Value.Materials.Num(); ++matIdx)
			{
				Component.Value.StaticMesh->SetMaterial(matIdx, Component.Value.Materials[matIdx]);
			}
		}
	}
}

void UCameraAdBlockerComponent::CalculateDistanceToTarget()
{
	FVector Start = ViewCamera->GetComponentLocation();
	FVector End = ViewTarget->GetComponentLocation();
	Distance = (float)(End - Start).Length();
}
