// Copyright TanZQ


#include "Player/GasPlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/GasAbilitySystemComponent.h"

AGasPlayerState::AGasPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UGasAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AGasPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
