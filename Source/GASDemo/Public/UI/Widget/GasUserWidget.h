// Copyright TanZQ

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GasUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class GASDEMO_API UGasUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
	
	
};
