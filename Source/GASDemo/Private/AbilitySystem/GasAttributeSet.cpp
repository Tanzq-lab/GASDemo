// Copyright TanZQ


#include "AbilitySystem/GasAttributeSet.h"

#include "Net/UnrealNetwork.h"

UGasAttributeSet::UGasAttributeSet()
{
	InitHealth(600);
	InitMaxHealth(1000);
}

void UGasAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Secondary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UGasAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

	// Vital Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UGasAttributeSet, Health, COND_None, REPNOTIFY_Always);
}

void UGasAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGasAttributeSet, Health, OldHealth);
}

void UGasAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGasAttributeSet, MaxHealth, OldMaxHealth);
}
