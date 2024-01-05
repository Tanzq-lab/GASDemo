// Copyright Amos Tan


#include "Player/GasPlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/GasAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/GasAmmoAttributeSet.h"
#include "AbilitySystem/AttributeSet/GasAttributeSet.h"
#include "Net/UnrealNetwork.h"

AGasPlayerState::AGasPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UGasAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	GasAttributeSet = CreateDefaultSubobject<UGasAttributeSet>("GasAttributeSet");
	GasAmmoAttributeSet = CreateDefaultSubobject<UGasAmmoAttributeSet>("GasAmmoAttributeSet");
	
	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AGasPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGasPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGasPlayerState, AttributePoints);
}

void AGasPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}
