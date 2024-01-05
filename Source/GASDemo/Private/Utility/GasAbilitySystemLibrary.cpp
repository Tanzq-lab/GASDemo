// Copyright Amos Tan


#include "Utility/GasAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "GameplayAbilitySpecHandle.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "AbilitySystem/GasGameplayAbility.h"
#include "Kismet/GameplayStatics.h"
#include "Player/GasPlayerState.h"
#include "UI/HUD/GasHUD.h"
#include "UI/WidgetController/GasWidgetController.h"
#include "AbilitySystem/AttributeSet/GasAttributeSet.h"

FString UGasAbilitySystemLibrary::GetPlayerEditorWindowRole(UWorld* World)
{
	FString Prefix;
	if (World)
	{
		if (World->WorldType == EWorldType::PIE)
		{
			switch (World->GetNetMode())
			{
			case NM_Client:
				Prefix = FString::Printf(TEXT("Client %d "), GPlayInEditorID - 1);
				break;
			case NM_DedicatedServer:
			case NM_ListenServer:
				Prefix = FString::Printf(TEXT("Server "));
				break;
			case NM_Standalone:
				break;
			}
		}
	}

	return Prefix;
}

bool UGasAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject,
	FWidgetControllerParams& OutWCParams, AGasHUD*& OutAuraHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutAuraHUD = Cast<AGasHUD>(PC->GetHUD());
		if (OutAuraHUD)
		{
			auto PS = PC->GetPlayerState<AGasPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UGasAttributeSet* AS = PS->GetGasAttributeSet();

			OutWCParams.AttributeSet = AS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.PlayerState = PS;
			OutWCParams.PlayerController = PC;
			return true;
		}
	}
	return false;
}

UAttributeMenuWidgetController* UGasAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AGasHUD* GasHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, GasHUD))
	{
		return GasHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

UGasGameplayAbility* UGasAbilitySystemLibrary::GetPrimaryAbilityInstanceFromHandle(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle)
{
	if (AbilitySystemComponent)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromHandle(Handle))
		{
			return Cast<UGasGameplayAbility>(AbilitySpec->GetPrimaryInstance());
		}
	}

	return nullptr;
}

UGasGameplayAbility* UGasAbilitySystemLibrary::GetPrimaryAbilityInstanceFromClass(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayAbility> InAbilityClass)
{
	if (AbilitySystemComponent)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(InAbilityClass))
		{
			return Cast<UGasGameplayAbility>(AbilitySpec->GetPrimaryInstance());
		}
	}

	return nullptr;
}

bool UGasAbilitySystemLibrary::IsPrimaryAbilityInstanceActive(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle)
{
	if (AbilitySystemComponent)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromHandle(Handle))
		{
			return Cast<UGasGameplayAbility>(AbilitySpec->GetPrimaryInstance())->IsActive();
		}
	}

	return false;
}

bool UGasAbilitySystemLibrary::IsAbilitySpecHandleValid(FGameplayAbilitySpecHandle Handle)
{
	return Handle.IsValid();
}

// bool UGasAbilitySystemLibrary::DoesEffectContainerSpecHaveEffects(const FGSGameplayEffectContainerSpec& ContainerSpec)
// {
// 	return ContainerSpec.HasValidEffects();
// }
//
// bool UGasAbilitySystemLibrary::DoesEffectContainerSpecHaveTargets(const FGSGameplayEffectContainerSpec& ContainerSpec)
// {
// 	return ContainerSpec.HasValidTargets();
// }
//
// void UGasAbilitySystemLibrary::ClearEffectContainerSpecTargets(FGSGameplayEffectContainerSpec& ContainerSpec)
// {
// 	ContainerSpec.ClearTargets();
// }
//
// void UGasAbilitySystemLibrary::AddTargetsToEffectContainerSpec(FGSGameplayEffectContainerSpec& ContainerSpec, const TArray<FGameplayAbilityTargetDataHandle>& TargetData, const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors)
// {
// 	ContainerSpec.AddTargets(TargetData, HitResults, TargetActors);
// }

// TArray<FActiveGameplayEffectHandle> UGasAbilitySystemLibrary::ApplyExternalEffectContainerSpec(const FGSGameplayEffectContainerSpec& ContainerSpec)
// {
// 	TArray<FActiveGameplayEffectHandle> AllEffects;
//
// 	// Iterate list of gameplay effects
// 	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
// 	{
// 		if (SpecHandle.IsValid())
// 		{
// 			// If effect is valid, iterate list of targets and apply to all
// 			for (TSharedPtr<FGameplayAbilityTargetData> Data : ContainerSpec.TargetData.Data)
// 			{
// 				AllEffects.Append(Data->ApplyGameplayEffectSpec(*SpecHandle.Data.Get()));
// 			}
// 		}
// 	}
// 	return AllEffects;
// }

// FGameplayAbilityTargetDataHandle UGasAbilitySystemLibrary::EffectContextGetTargetData(FGameplayEffectContextHandle EffectContextHandle)
// {
// 	FGSGameplayEffectContext* EffectContext = static_cast<FGSGameplayEffectContext*>(EffectContextHandle.Get());
// 	
// 	if (EffectContext)
// 	{
// 		return EffectContext->GetTargetData();
// 	}
//
// 	return FGameplayAbilityTargetDataHandle();
// }
//
// void UGasAbilitySystemLibrary::EffectContextAddTargetData(FGameplayEffectContextHandle EffectContextHandle, const FGameplayAbilityTargetDataHandle& TargetData)
// {
// 	FGSGameplayEffectContext* EffectContext = static_cast<FGSGameplayEffectContext*>(EffectContextHandle.Get());
//
// 	if (EffectContext)
// 	{
// 		EffectContext->AddTargetData(TargetData);
// 	}
// }

void UGasAbilitySystemLibrary::ClearTargetData(FGameplayAbilityTargetDataHandle& TargetData)
{
	TargetData.Clear();
}
