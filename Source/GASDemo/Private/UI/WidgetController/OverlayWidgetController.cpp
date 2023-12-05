// Copyright Amos Tan

#include "UI/WidgetController/OverlayWidgetController.h"

// #include "GasGameplayTags.h"
#include "AbilitySystem/GasAbilitySystemComponent.h"
// #include "AbilitySystem/GasAttributeSet.h"
// #include "AbilitySystem/Data/AbilityInfo.h"
// #include "AbilitySystem/Data/LevelUpInfo.h"
#include "AbilitySystem/AttributeSet/GasAttributeSet.h"
#include "Player/GasPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{

	OnHealthChanged.Broadcast(GetGasAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetGasAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetGasAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetGasAS()->GetMaxMana());
	
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetGasAS()->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetGasAS()->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetGasAS()->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetGasAS()->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);

	if (GetGasASC())
	{
		GetGasASC()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& Tag : AssetTags)
				{
					// 例如 Tag = Message.HealthPotion
					// "Message.HealthPotion".MatchesTag("Message") 返回 True
					// "Message".MatchesTag("Message.HealthPotion") 返回 False
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag))
					{
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}
	
}

//
// void UOverlayWidgetController::BindCallbacksToDependencies()
// {
// 	GetGasPS()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
// 	GetGasPS()->OnLevelChangedDelegate.AddLambda(
// 		[this](int32 NewLevel, bool bLevelUp)
// 		{
// 			OnPlayerLevelChangedDelegate.Broadcast(NewLevel, bLevelUp);
// 		}
// 	);
// 	
// 	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetGasAS()->GetHealthAttribute()).AddLambda(
// 			[this](const FOnAttributeChangeData& Data)
// 			{
// 				OnHealthChanged.Broadcast(Data.NewValue);
// 			}
// 		);
// 	
// 	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetGasAS()->GetMaxHealthAttribute()).AddLambda(
// 			[this](const FOnAttributeChangeData& Data)
// 			{
// 				OnMaxHealthChanged.Broadcast(Data.NewValue);
// 			}
// 		);
// 	
// 	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetGasAS()->GetManaAttribute()).AddLambda(
// 			[this](const FOnAttributeChangeData& Data)
// 			{
// 				OnManaChanged.Broadcast(Data.NewValue);
// 			}
// 		);
// 	
// 	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetGasAS()->GetMaxManaAttribute()).AddLambda(
// 			[this](const FOnAttributeChangeData& Data)
// 			{
// 				OnMaxManaChanged.Broadcast(Data.NewValue);
// 			}
// 		);
// 	
// 	if (GetGasASC())
// 	{
// 		GetGasASC()->AbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
// 		if (GetGasASC()->bStartupAbilitiesGiven)
// 		{
// 			BroadcastAbilityInfo();
// 		}
// 		else
// 		{
// 			GetGasASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
// 		}
// 	
// 		GetGasASC()->EffectAssetTags.AddLambda(
// 			[this](const FGameplayTagContainer& AssetTags)
// 			{
// 				for (const FGameplayTag& Tag : AssetTags)
// 				{
// 					// For example, say that Tag = Message.HealthPotion
// 					// "Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False
// 					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
// 					if (Tag.MatchesTag(MessageTag))
// 					{
// 						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
// 						MessageWidgetRowDelegate.Broadcast(*Row);
// 					}
// 				}
// 			}
// 		);
// 	}
// 	
// }

// void UOverlayWidgetController::OnXPChanged(int32 NewXP)
// {
// 	const ULevelUpInfo* LevelUpInfo = GetGasPS()->LevelUpInfo;
// 	checkf(LevelUpInfo, TEXT("Unabled to find LevelUpInfo. Please fill out GasPlayerState Blueprint"));
//
// 	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
// 	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();
//
// 	if (Level <= MaxLevel && Level > 0)
// 	{
// 		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
// 		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;
//
// 		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
// 		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;
//
// 		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);
//
// 		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
// 	}
// }
//
// void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
// {
// 	const FGasGameplayTags& GameplayTags = FGasGameplayTags::Get();
// 	
// 	FGasAbilityInfo LastSlotInfo;
// 	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
// 	LastSlotInfo.InputTag = PreviousSlot;
// 	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
// 	// Broadcast empty info if PreviousSlot is a valid slot. Only if equipping an already-equipped spell
// 	AbilityInfoDelegate.Broadcast(LastSlotInfo);
//
// 	FGasAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
// 	Info.StatusTag = Status;
// 	Info.InputTag = Slot;
// 	AbilityInfoDelegate.Broadcast(Info);
// }
