// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "GasUserWidget.h"
#include "OverlayUserWidget.generated.h"

class UReticleWidget;

/**
 * 
 */
UCLASS()
class GASDEMO_API UOverlayUserWidget : public UGasUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetReticle(TSubclassOf<UReticleWidget> ReticleClass);
	
};
