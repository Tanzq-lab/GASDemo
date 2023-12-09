// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GasDamageTextWidgetComponent.generated.h"


struct FGameplayTagContainer;

UCLASS()
class GASDEMO_API UGasDamageTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage, const FGameplayTagContainer& Tags);
};
