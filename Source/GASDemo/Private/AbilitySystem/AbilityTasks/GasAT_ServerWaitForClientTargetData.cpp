// Copyright Amos Tan


#include "AbilitySystem/AbilityTasks/GasAT_ServerWaitForClientTargetData.h"

#include "AbilitySystemComponent.h"


UGasAT_ServerWaitForClientTargetData::UGasAT_ServerWaitForClientTargetData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bTriggerOnce(false)
{
}

UGasAT_ServerWaitForClientTargetData* UGasAT_ServerWaitForClientTargetData::ServerWaitForClientTargetData(UGameplayAbility* OwningAbility, FName TaskInstanceName, bool TriggerOnce)
{
	UGasAT_ServerWaitForClientTargetData* MyObj = NewAbilityTask<UGasAT_ServerWaitForClientTargetData>(OwningAbility, TaskInstanceName);
	MyObj->bTriggerOnce = TriggerOnce;
	return MyObj;
}

void UGasAT_ServerWaitForClientTargetData::Activate()
{
	if (!Ability || !Ability->GetCurrentActorInfo()->IsNetAuthority())
	{
		return;
	}

	const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
	const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
	AbilitySystemComponent->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UGasAT_ServerWaitForClientTargetData::OnTargetDataReplicatedCallback);
}

void UGasAT_ServerWaitForClientTargetData::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& Data, FGameplayTag ActivationTag)
{
	FGameplayAbilityTargetDataHandle MutableData = Data;
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(MutableData);
	}

	if (bTriggerOnce)
	{
		EndTask();
	}
}

void UGasAT_ServerWaitForClientTargetData::OnDestroy(bool AbilityEnded)
{
	if (AbilitySystemComponent.IsValid())
	{
		FGameplayAbilitySpecHandle	SpecHandle = GetAbilitySpecHandle();
		FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).RemoveAll(this);
	}

	Super::OnDestroy(AbilityEnded);
}
