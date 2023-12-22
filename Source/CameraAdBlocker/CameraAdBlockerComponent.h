// @ DimmaK, Kyiv, Ukraine, Camera controller project december 2023.

#pragma once

//? C++ std
#include <algorithm>

//? UnrealEngine
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

//* Place this header bellow every other headers
#include "CameraAdBlockerComponent.generated.h"

USTRUCT(BlueprintType)
struct FCameraBlocker
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterialInterface*> Materials;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bToModify;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CAMERAADBLOCKER_API UCameraAdBlockerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	//? Sets default values for this component's properties
	UCameraAdBlockerComponent();

protected:
	//? Called when the game starts
	virtual void BeginPlay() override;

public:
	//? Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	//@ API

	//? Helps to parse all necessities to component to work
	void Launch(class USceneComponent* Target, class UCapsuleComponent* Capsule, class UCameraComponent* Camera);

	//@ Properties

	//? That's awy we can enable or disable component in the editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Component")
	bool Enabled{ true };

	//? Material that would be used when occlusion happens
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|Materials")
	class UMaterialInterface* FadeMaterial{ nullptr };

	//? How much of the Pawn capsule Radius and Height
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|Capsule",
			  meta = (ClampMin = "0.001", ClampMax = "10.0"))
	float CapsulePercentageForTrace{ 0.5f };

	//? How fae is aim object?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|View",
			  meta = (ClampMin = "50.0", ClampMax = "50000.0"))
	float ViewDistance{ 500.f };

	//? How far mesh from camera when not modified
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|View",
			  meta = (ClampMin = "180.0", ClampMax = "300.0"))
	float OcclusionDistance{ 230.f };

	//? How far mesh from camera when it's should be transparent
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|View",
			  meta = (ClampMin = "80.0", ClampMax = "160.0"))
	float MinOcclusionDistance{ 140.f };

	//? Change to 'true' when debugging
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|Debugging")
	bool bDebugLineTraces{ false };

	//? Maybe no need to make mesh 100% transparent
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|Opacity",
			  meta = (ClampMin = "0.01", ClampMax = "1.0"))
	float MinOpacity{ 0.1f };

	//? FRom what opacity we start to modify material
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|Opacity",
			  meta = (ClampMin = "0.01", ClampMax = "1.0"))
	float MaxOpacity{ 1.f };

private:
	//@ Methods

	//? Maybe some new meshes need to modify or rollback already modified
	void CheckOccludedMeshes();
	//? Apply new material when occlusion happend or return originals
	void OnShowHide();
	//? We need to know how far camera from nozzle to calculate opacity later
	void CalculateDistanceToTarget();
	//? Calling a lot, inline is usefull
	__forceinline void CalculateNewOpacity()
	{
		NewOpacity = (Distance - MinOcclusionDistance) / (OcclusionDistance - MinOcclusionDistance);
		NewOpacity = std::clamp(NewOpacity, MinOpacity, MaxOpacity);
	}

	//@ Necessities

	//? Where we looking
	class USceneComponent* ViewTarget{ nullptr };
	//? What tells us that we block view
	class UCapsuleComponent* ViewClearance{ nullptr };
	//? That is how we see
	class UCameraComponent* ViewCamera{ nullptr };

	//@ Utilities

	//? Do pawn parse to us all necessitiies?
	bool bInit{ false };
	//? Container for current trackable meshes
	TMap<const UStaticMeshComponent*, FCameraBlocker> BlockingMeshes;
	//? Distance to from camera to nozzle
	float Distance{};
	//? Opacity for overwritable material
	float NewOpacity{};
};
