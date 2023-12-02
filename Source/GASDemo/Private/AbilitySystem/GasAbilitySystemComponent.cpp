// Copyright Amos Tan


#include "AbilitySystem/GasAbilitySystemComponent.h"

UGasAbilitySystemComponent::UGasAbilitySystemComponent()
{
}

void UGasAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UGasAbilitySystemComponent::ClientEffectApplied);
}

void UGasAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}

