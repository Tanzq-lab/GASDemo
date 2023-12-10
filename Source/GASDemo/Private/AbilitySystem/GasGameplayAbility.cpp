// Copyright Amos Tan


#include "AbilitySystem/GasGameplayAbility.h"

#include "AbilitySystem/GasAbilitySystemComponent.h"
#include "Character/GasCharacterBase.h"
#include "Utility/GasTargetType.h"

UGasGameplayAbility::UGasGameplayAbility()
{
	// Default to Instance Per Actor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UGasGameplayAbility::BatchRPCTryActivateAbility(FGameplayAbilitySpecHandle InAbilityHandle,
                                                     bool EndAbilityImmediately)
{
	if (UGasAbilitySystemComponent* GasASC = Cast<UGasAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
	{
		return GasASC->BatchRPCTryActivateAbility(InAbilityHandle, EndAbilityImmediately);
	}

	return false;
}

void UGasGameplayAbility::ExternalEndAbility()
{
	check(CurrentActorInfo);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

FGasGameplayEffectContainerSpec UGasGameplayAbility::MakeEffectContainerSpec(FGameplayTag ContainerTag,
	const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	if (FGasGameplayEffectContainer* FoundContainer = EffectContainerMap.Find(ContainerTag))
	{
		return MakeEffectContainerSpecFromContainer(*FoundContainer, EventData, OverrideGameplayLevel);
	}
	return FGasGameplayEffectContainerSpec();
}

FGasGameplayEffectContainerSpec UGasGameplayAbility::MakeEffectContainerSpecFromContainer(
	const FGasGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	// First figure out our actor info
	FGasGameplayEffectContainerSpec ReturnSpec;
	const AActor* OwningActor = GetOwningActorFromActorInfo();
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	AGasCharacterBase* AvatarCharacter = Cast<AGasCharacterBase>(AvatarActor);

	if (UGasAbilitySystemComponent* OwningASC = UGasAbilitySystemComponent::GetAbilitySystemComponentFromActor(OwningActor))
	{
		// If we have a target type, run the targeting logic. This is optional, targets can be added later
		if (Container.TargetType.Get())
		{
			TArray<FHitResult> HitResults;
			TArray<AActor*> TargetActors;
			TArray<FGameplayAbilityTargetDataHandle> TargetData;
			const UGasTargetType* TargetTypeCDO = Container.TargetType.GetDefaultObject();
			TargetTypeCDO->GetTargets(AvatarCharacter, AvatarActor, EventData, TargetData, HitResults, TargetActors);
			ReturnSpec.AddTargets(TargetData, HitResults, TargetActors);	
		}

		// If we don't have an override level, use the ability level
		if (OverrideGameplayLevel == INDEX_NONE)
		{
			//OverrideGameplayLevel = OwningASC->GetDefaultAbilityLevel();
			OverrideGameplayLevel = GetAbilityLevel();
		}

		// Build GameplayEffectSpecs for each applied effect
		for (const TSubclassOf<UGameplayEffect>& EffectClass : Container.TargetGameplayEffectClasses)
		{
			ReturnSpec.TargetGameplayEffectSpecs.Add(MakeOutgoingGameplayEffectSpec(EffectClass, OverrideGameplayLevel));
		}
	}
	return ReturnSpec;
}

UObject* UGasGameplayAbility::K2_GetSourceObject(FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo& ActorInfo) const
{
	return GetSourceObject(Handle, &ActorInfo);
}

TArray<FActiveGameplayEffectHandle> UGasGameplayAbility::ApplyEffectContainerSpec(
	const FGasGameplayEffectContainerSpec& ContainerSpec)
{
	TArray<FActiveGameplayEffectHandle> AllEffects;

	// Iterate list of effect specs and apply them to their target data
	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
	{
		AllEffects.Append(K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TargetData));
	}
	return AllEffects;
}

void UGasGameplayAbility::SetCurrentMontageForMesh(USkeletalMeshComponent* InMesh, UAnimMontage* InCurrentMontage)
{
	ensure(IsInstantiated());

	FAbilityMeshMontage AbilityMeshMontage;
	if (FindAbilityMeshMontage(InMesh, AbilityMeshMontage))
	{
		AbilityMeshMontage.Montage = InCurrentMontage;
	}
	else
	{
		CurrentAbilityMeshMontages.Add(FAbilityMeshMontage(InMesh, InCurrentMontage));
	}
}

bool UGasGameplayAbility::FindAbilityMeshMontage(USkeletalMeshComponent* InMesh, FAbilityMeshMontage& InAbilityMeshMontage)
{
	for (const FAbilityMeshMontage& MeshMontage : CurrentAbilityMeshMontages)
	{
		if (MeshMontage.Mesh == InMesh)
		{
			InAbilityMeshMontage = MeshMontage;
			return true;
		}
	}

	return false;
}

FString UGasGameplayAbility::GetCurrentPredictionKeyStatus()
{
	const UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	return ASC->ScopedPredictionKey.ToString() + " is valid for more prediction: " + (ASC->ScopedPredictionKey.IsValidForMorePrediction() ? TEXT("true") : TEXT("false"));
}

bool UGasGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	// if (bCannotActivateWhileInteracting)
	// {
	// 	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	// 	if (ASC->GetTagCount(InteractingTag) > ASC->GetTagCount(InteractingRemovalTag))
	// 	{
	// 		return false;
	// 	}
	// }
	//
	// if (bSourceObjectMustEqualCurrentWeaponToActivate)
	// {
	// 	AGSHeroCharacter* Hero = Cast<AGSHeroCharacter>(ActorInfo->AvatarActor);
	// 	if (Hero && Hero->GetCurrentWeapon() && (UObject*)Hero->GetCurrentWeapon() == GetSourceObject(Handle, ActorInfo))
	// 	{
	// 		return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	// 	}
	// 	else
	// 	{
	// 		return false;
	// 	}
	// }

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

bool UGasGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags) && GasCheckCost(Handle, *ActorInfo);
}

bool UGasGameplayAbility::GasCheckCost_Implementation(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo& ActorInfo) const
{
	return true;
}

void UGasGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	GasApplyCost(Handle, *ActorInfo, ActivationInfo);
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);
}

void UGasGameplayAbility::GasApplyCost_Implementation(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo& ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
}
