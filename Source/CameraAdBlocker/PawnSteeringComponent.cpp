// @ DimmaK, Kyiv, Ukraine, Camera controller project december 2023.

//? Source code
#include "PawnSteeringComponent.h"

UPawnSteeringComponent::UPawnSteeringComponent()
{
	//? Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
	//? features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UPawnSteeringComponent::BeginPlay() { Super::BeginPlay(); }

void UPawnSteeringComponent::TickComponent(float DeltaTime, ELevelTick TickType,
										   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PrimitiveRoot)
	{
		FVector CurrentAngularVelocity = PrimitiveRoot->GetPhysicsAngularVelocityInDegrees();
		FVector UpVector = PrimitiveRoot->GetUpVector();
		float CurrentAngularSpeed = (float)FVector::DotProduct(CurrentAngularVelocity, UpVector);

		FVector Torque;

		if (TargetAngularSpeed > 1.f || TargetAngularSpeed < -1.f)
		{
			Torque =
				FVector(0, 0, 1.0) * ((TargetAngularSpeed - CurrentAngularSpeed) * AngularAcceleration * DeltaTime);
		}
		else
		{
			Torque =
				FVector(0, 0, 1.0) * ((TargetAngularSpeed - CurrentAngularSpeed) * AngularDeceleration * DeltaTime);
		}

		// PrimitiveRoot->WakeRigidBody();
		PrimitiveRoot->AddTorqueInDegrees(Torque, TEXT("NAME_None"), true);
	}
	else
	{
		// UE_LOG(LogTemp, Warning, TEXT("NO ROOT!"));
	}
}

void UPawnSteeringComponent::IncrementSpeed(float Input)
{
	if (Input)
	{
		TargetAngularSpeed += Input * AngularSpeedMultiplier;
		TargetAngularSpeed = FMath::Clamp(TargetAngularSpeed, -AngularSpeedLimit, AngularSpeedLimit);
	}
	else
	{
		TargetAngularSpeed = 0;
	}
}
