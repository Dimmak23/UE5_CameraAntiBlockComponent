// @ DimmaK, Kyiv, Ukraine, Camera controller project december 2023.

//? Source code
#include "AdBlockerController.h"

//? C++ std

//? UnrealEngine
// #include "Engine/EngineTypes.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

//? Controller project headers

//?
static void _MapKey(										   //
	UInputMappingContext* InputMappingContext,				   //
	UInputAction* InputAction,								   //
	FKey Key,												   //
	bool Negate = false,									   //
	bool Swizzle = false,									   //
	EInputAxisSwizzle SwizzleOrder = EInputAxisSwizzle::YXZ	   //
)
{
	FEnhancedActionKeyMapping& _Mapping = InputMappingContext->MapKey(InputAction, Key);
	UObject* _Outer = InputMappingContext->GetOuter();

	if (Negate)
	{
		UInputModifierNegate* _Negate = NewObject<UInputModifierNegate>(_Outer);
		_Mapping.Modifiers.Add(_Negate);
	}

	if (Swizzle)
	{
		UInputModifierSwizzleAxis* _Swizzle = NewObject<UInputModifierSwizzleAxis>(_Outer);
		_Swizzle->Order = SwizzleOrder;
		_Mapping.Modifiers.Add(_Swizzle);
	}
}

AAdBlockerController::AAdBlockerController() {}

void AAdBlockerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//? Create these objects here and not in the constructor because we only need them on the client
	PawnMappingContext = NewObject<UInputMappingContext>(this);

	//? Handle move inputs from player
	MovePawnAction = NewObject<UInputAction>(this);
	MovePawnAction->ValueType = EInputActionValueType::Axis1D;
	_MapKey(PawnMappingContext, MovePawnAction, EKeys::W);
	_MapKey(PawnMappingContext, MovePawnAction, EKeys::S, true);

	//? Handle steer inputs from player
	SteerPawnAction = NewObject<UInputAction>(this);
	SteerPawnAction->ValueType = EInputActionValueType::Axis1D;
	_MapKey(PawnMappingContext, SteerPawnAction, EKeys::A, true);
	_MapKey(PawnMappingContext, SteerPawnAction, EKeys::D);

	//? Handle rotate camera input from  player
	RotateCameraAction = NewObject<UInputAction>(this);
	RotateCameraAction->ValueType = EInputActionValueType::Axis2D;
	_MapKey(PawnMappingContext, RotateCameraAction, EKeys::MouseY);
	_MapKey(PawnMappingContext, RotateCameraAction, EKeys::MouseX, false, true);
}

void AAdBlockerController::BeginPlay() { Super::BeginPlay(); }
