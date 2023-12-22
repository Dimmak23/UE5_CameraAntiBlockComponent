// @ DimmaK, Kyiv, Ukraine, Camera controller project december 2023.

#pragma once

//? C++ std

//? UnrealEngine
#include "CoreMinimal.h"
#include "PawnPhysics.h"

//* Place this header bellow every other headers
#include "PawnSteeringComponent.generated.h"

UCLASS(ClassGroup = (Custom), BlueprintType,
	   meta = (BlueprintSpawnableComponent, ShortTooltip = "A steering module to steer droid wheels."))
class CAMERAADBLOCKER_API UPawnSteeringComponent : public UPawnPhysics
{
	GENERATED_BODY()

public:
	//? Sets default values for this component's properties
	UPawnSteeringComponent();

protected:
	//? Called when the game starts
	virtual void BeginPlay() override;

public:
	//? Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	//@ API

	//? Invoke from parent tank
	virtual void IncrementSpeed(float Input) override;

private:
	//?
	float TargetAngularSpeed;

public:
	//?
	UPROPERTY(EditAnywhere, Category = "Steering Adjustments")
	float AngularSpeedMultiplier{ 12.5f };	  // 12.5
	//?
	UPROPERTY(EditAnywhere, Category = "Steering Adjustments")
	float AngularSpeedLimit{ 250.f };	 // 250
	//?
	UPROPERTY(EditAnywhere, Category = "Steering Adjustments")
	float AngularAcceleration{ 480.f };	   // 480
	//?
	UPROPERTY(EditAnywhere, Category = "Steering Adjustments")
	float AngularDeceleration{ 300.f };
};
