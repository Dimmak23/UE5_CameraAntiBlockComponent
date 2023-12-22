//@ DimmaK, Kyiv, Ukraine, Camera controller project december 2023.

#pragma once

//? C++ std

//? UnrealEngine
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

//* Place this header bellow every other headers
#include "CameraAdBlockerGameModeBase.generated.h"

UCLASS(ClassGroup = (Custom), BlueprintType,
	   meta = (BlueprintSpawnableComponent, ShortTooltip = "Managing gameplay modes."))
class CAMERAADBLOCKER_API ACameraAdBlockerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	//?
	virtual void BeginPlay() override;
};
