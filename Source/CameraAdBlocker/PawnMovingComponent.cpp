// @ DimmaK, Kyiv, Ukraine, Camera controller project december 2023.

//? Source code
#include "PawnMovingComponent.h"

//? C++ std
#include <algorithm>
#include <cstdlib>

//? UnrealEngine

UPawnMovingComponent::UPawnMovingComponent()
{
	//? Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
	//? features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UPawnMovingComponent::BeginPlay() { Super::BeginPlay(); }

void UPawnMovingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
										 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PrimitiveRoot)
	{
		FVector CurrentVelocity = PrimitiveRoot->GetPhysicsLinearVelocity();
		FVector ForwardVector = PrimitiveRoot->GetForwardVector();
		float CurrentSpeed = (float)FVector::DotProduct(CurrentVelocity, ForwardVector);

		FVector Force;

		if (TargetLinearSpeed > 1.f || TargetLinearSpeed < -1.f)
		{
			Force = ForwardVector * ((TargetLinearSpeed - CurrentSpeed) * LinearAcceleration * DeltaTime);
		}
		else
		{
			Force = ForwardVector * ((TargetLinearSpeed - CurrentSpeed) * LinearDeceleration * DeltaTime);
		}

		PrimitiveRoot->AddForce(Force);
	}
	else
	{
		// # UE_LOG(LogTemp, Warning, TEXT("NO ROOT!"));
	}
}

void UPawnMovingComponent::IncrementSpeed(float Input)
{
	if (Input)
	{
		TargetLinearSpeed += Input * LinearSpeedMultiplier;
		TargetLinearSpeed = FMath::Clamp(	 //
			TargetLinearSpeed,				 //
			-LinearSpeedLimit,				 //
			LinearSpeedLimit				 //
		);
	}
	else
	{
		TargetLinearSpeed = 0;
	}
}
