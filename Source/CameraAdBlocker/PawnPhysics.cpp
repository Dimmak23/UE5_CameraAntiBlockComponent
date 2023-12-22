// @ DimmaK, Kyiv, Ukraine, Camera controller project december 2023.

#include "PawnPhysics.h"

//? Sets default values for this component's properties
UPawnPhysics::UPawnPhysics()
{
	//? Set this component to be initialized when the game starts, and to be ticked every frame.
	//? You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

//? Called when the game starts
void UPawnPhysics::BeginPlay() { Super::BeginPlay(); }

void UPawnPhysics::SetRoot(UPrimitiveComponent* NewRoot) { PrimitiveRoot = NewRoot; }

//? Called every frame
void UPawnPhysics::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPawnPhysics::IncrementSpeed(float Input) {}
