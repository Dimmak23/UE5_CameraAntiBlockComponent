// @ DimmaK, Kyiv, Ukraine, Camera controller project december 2023.

#pragma once

//? C++ std

//? UnrealEngine
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

//* Place this header bellow every other headers
#include "DroidPawnBase.generated.h"

UCLASS()
class CAMERAADBLOCKER_API ADroidPawnBase : public APawn
{
	GENERATED_BODY()

public:
	//? Sets default values for this pawn's properties
	ADroidPawnBase();

	//? Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//? Called every frame
	virtual void Tick(float DeltaTime) override;

	//? Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	//@ Base

	//? Mass box
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* MassOrigin{ nullptr };

	//? Base
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh{ nullptr };

	//? Turret
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh{ nullptr };

	//? Nozzle
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SpawnPoint{ nullptr };

	//? What tells us that we block view
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* ViewClearance{ nullptr };

	//@ Camera

	//? Spring arm for camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm{ nullptr };

	//? Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera{ nullptr };

	//? Scale to apply to rotation input
	UPROPERTY(EditDefaultsOnly, Category = "Camera rotation")
	float RotateScale{ 150.f };

	//? Camera minimum pitch angle
	UPROPERTY(EditDefaultsOnly, Category = "Camera rotation")
	float CameraMinimumPitch{ -15.f };

	//? Camera maximum pitch angle
	UPROPERTY(EditDefaultsOnly, Category = "Camera rotation")
	float CameraMaximumPitch{ 0.f };

	//? How fast third person camera adapts player input
	UPROPERTY(EditDefaultsOnly, Category = "Camera rotation")
	float CameraRotationSpeed{ 18.f };

	//@ Engine

	//? Module for moving
	UPROPERTY(EditDefaultsOnly, Category = "Mover")
	class UPawnMovingComponent* EngineComponent{ nullptr };

	//@ Wheels

	//? Module for steerint
	UPROPERTY(EditDefaultsOnly, Category = "Wheels")
	class UPawnSteeringComponent* SteerComponent{ nullptr };

	//@ Turret

	//? How fast turret adapts to player camera rotation or enemy aiming
	UPROPERTY(EditDefaultsOnly, Category = "Turret abilities")
	float TurretRotationSpeed{ 7.f };

	//@ AdBlocker

	//?
	UPROPERTY(EditDefaultsOnly, Category = "Anti blocking feature")
	class UCameraAdBlockerComponent* AdBlocker{ nullptr };

private:
	//? Handle turret rotation
	void RotateTurret();
	//? Handle input to move pawn
	void Move(const struct FInputActionValue& ActionValue);
	//? Handle input to steer pawn
	void Steer(const struct FInputActionValue& ActionValue);
	//? Handle input to rotate camera
	void RotateCamera(const struct FInputActionValue& ActionValue);
};
