// Copyright Amos Tan


#include "AbilitySystem/GasAbilitySystemComponent.h"

#include "Input/GasInputMappingContext.h"

UGasAbilitySystemComponent::UGasAbilitySystemComponent(): bCharacterAbilitiesGiven(false)
{
}

void UGasAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UGasAbilitySystemComponent::ClientEffectApplied);
}

void UGasAbilitySystemComponent::BindAbilityToInputComponent(UInputComponent* InputComponent,
	TArray<FGasInputMapping>& InputMappings)
{
	for(const auto& InputMap : InputMappings)
	{
		int32 Idx = static_cast<int32>(InputMap.InputID);
		for (const auto &Key : InputMap.Keys)
		{
			// IE_Pressed
			if (InputMap.bPressed)
			{
				FInputKeyBinding KB(FInputChord(Key, false, false, false, false), IE_Pressed);
				KB.KeyDelegate.GetDelegateForManualSet().BindUObject(this, &UAbilitySystemComponent::AbilityLocalInputPressed, Idx );
				InputComponent->KeyBindings.Emplace(MoveTemp(KB));
			}

			// IE_Released
			if (InputMap.bReleased)
			{
				FInputKeyBinding KB(FInputChord(Key, false, false, false, false), IE_Released);
				KB.KeyDelegate.GetDelegateForManualSet().BindUObject(this, &UAbilitySystemComponent::AbilityLocalInputPressed, Idx );
				InputComponent->KeyBindings.Emplace(MoveTemp(KB));
			}
		}
	}
}

void UGasAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                    const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}

