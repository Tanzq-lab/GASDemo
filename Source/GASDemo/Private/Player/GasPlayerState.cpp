// Copyright Amos Tan


#include "Player/GasPlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/GasAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/GasAttributeSet.h"

AGasPlayerState::AGasPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UGasAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UGasAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AGasPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
