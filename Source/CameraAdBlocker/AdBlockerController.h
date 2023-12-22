// @ DimmaK, Kyiv, Ukraine, Camera controller project december 2023.

#pragma once

//? C++ std

//? UnrealEngine
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

//* Place this header bellow every other headers
#include "AdBlockerController.generated.h"

UCLASS()
class CAMERAADBLOCKER_API AAdBlockerController : public APlayerController
{
	GENERATED_BODY()

public:
	//?
	AAdBlockerController();

	//@ API

	//? Allows the PlayerController to set up custom input bindings
	virtual void SetupInputComponent() override;

protected:
	//? Called when the game starts
	virtual void BeginPlay() override;

public:
	//@ PROPERTIES

	//? Mapping context used for pawn control
	UPROPERTY()
	class UInputMappingContext* PawnMappingContext;
	//? Action to move forward
	UPROPERTY()
	class UInputAction* MovePawnAction;
	//? Action to steer to the some side
	UPROPERTY()
	class UInputAction* SteerPawnAction;
	//? Action to manipulate camera
	UPROPERTY()
	class UInputAction* RotateCameraAction;
};
