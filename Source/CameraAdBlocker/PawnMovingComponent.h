// @ DimmaK, Kyiv, Ukraine, Camera controller project december 2023.

#pragma once

//? C++ std

//? UnrealEngine
#include "CoreMinimal.h"
#include "PawnPhysics.h"

//* Place this header bellow every other headers
#include "PawnMovingComponent.generated.h"

UCLASS(ClassGroup = (Custom), BlueprintType,
	   meta = (BlueprintSpawnableComponent, ShortTooltip = "An engine module to move droid."))
class CAMERAADBLOCKER_API UPawnMovingComponent : public UPawnPhysics
{
	GENERATED_BODY()

public:
	//? Sets default values for this component's properties
	UPawnMovingComponent();

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
	float TargetLinearSpeed;

public:
	//@ Engine parameters

	//?
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Engine Adjustments")
	float LinearSpeedMultiplier{ 60.f };
	//?
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Engine Adjustments")
	float LinearSpeedLimit{ 900.f };
	//?
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Engine Adjustments")
	float LinearAcceleration{ 360000.f };
	//?
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Engine Adjustments")
	float LinearDeceleration{ 120.f };
};
