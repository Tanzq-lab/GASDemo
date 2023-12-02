// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GasAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);

UCLASS()
class GASDEMO_API UGasAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UGasAbilitySystemComponent();

	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;
	
protected:

	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

};
