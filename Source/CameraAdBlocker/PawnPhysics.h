// @ DimmaK, Kyiv, Ukraine, Camera controller project december 2023.

#pragma once

//? C++ std

//? UnrealEngine
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

//* Place this header bellow every other headers
#include "PawnPhysics.generated.h"

UCLASS(ClassGroup = (Custom), BlueprintType,
	   meta = (BlueprintSpawnableComponent, ShortTooltip = "Parent with pointer to the root component."))
class CAMERAADBLOCKER_API UPawnPhysics : public UActorComponent
{
	GENERATED_BODY()

public:
	//? Sets default values for this component's properties
	UPawnPhysics();

protected:
	//? Called when the game starts
	virtual void BeginPlay() override;

	//? Parent primitive component to apply force and torque
	class UPrimitiveComponent* PrimitiveRoot{ nullptr };

public:
	//? Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	//@ API

	//? Parse root
	void SetRoot(UPrimitiveComponent* NewRoot);

	//? Implements to apply speed
	//! Can we do pure virtual function?
	virtual void IncrementSpeed(float Input);
};
