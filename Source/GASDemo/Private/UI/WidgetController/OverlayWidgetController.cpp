// Copyright Amos Tan

#include "UI/WidgetController/OverlayWidgetController.h"

// #include "GasGameplayTags.h"
#include "AbilitySystem/GasAbilitySystemComponent.h"
// #include "AbilitySystem/GasAttributeSet.h"
// #include "AbilitySystem/Data/AbilityInfo.h"
// #include "AbilitySystem/Data/LevelUpInfo.h"
#include "AbilitySystem/AttributeSet/GasAttributeSet.h"
#include "Player/GasPlayerController.h"
#include "Player/GasPlayerState.h"
#include "Utility/GasAbilitySystemLibrary.h"

void UOverlayWidgetController::BroadcastInitialValues()
{

	OnHealthChanged.Broadcast(GetGasAttributeSet()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetGasAttributeSet()->GetMaxHealth());
	OnManaChanged.Broadcast(GetGasAttributeSet()->GetMana());
	OnMaxManaChanged.Broadcast(GetGasAttributeSet()->GetMaxMana());
	
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetGasAttributeSet()->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetGasAttributeSet()->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetGasAttributeSet()->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetGasAttributeSet()->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
	);

	if (GetGasAbilitySystemComponent())
	{
		GetGasAbilitySystemComponent()->EffectAssetTags.AddLambda(
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
						const FUIWidgetRow* Row = UGasAbilitySystemLibrary::GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
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
// 	GetGasPlayerState()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
// 	GetGasPlayerState()->OnLevelChangedDelegate.AddLambda(
// 		[this](int32 NewLevel, bool bLevelUp)
// 		{
// 			OnPlayerLevelChangedDelegate.Broadcast(NewLevel, bLevelUp);
// 		}
// 	);
// 	
// 	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetGasAttributeSet()->GetHealthAttribute()).AddLambda(
// 			[this](const FOnAttributeChangeData& Data)
// 			{
// 				OnHealthChanged.Broadcast(Data.NewValue);
// 			}
// 		);
// 	
// 	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetGasAttributeSet()->GetMaxHealthAttribute()).AddLambda(
// 			[this](const FOnAttributeChangeData& Data)
// 			{
// 				OnMaxHealthChanged.Broadcast(Data.NewValue);
// 			}
// 		);
// 	
// 	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetGasAttributeSet()->GetManaAttribute()).AddLambda(
// 			[this](const FOnAttributeChangeData& Data)
// 			{
// 				OnManaChanged.Broadcast(Data.NewValue);
// 			}
// 		);
// 	
// 	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetGasAttributeSet()->GetMaxManaAttribute()).AddLambda(
// 			[this](const FOnAttributeChangeData& Data)
// 			{
// 				OnMaxManaChanged.Broadcast(Data.NewValue);
// 			}
// 		);
// 	
// 	if (GetGasAbilitySystemComponent())
// 	{
// 		GetGasAbilitySystemComponent()->AbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
// 		if (GetGasAbilitySystemComponent()->bStartupAbilitiesGiven)
// 		{
// 			BroadcastAbilityInfo();
// 		}
// 		else
// 		{
// 			GetGasAbilitySystemComponent()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
// 		}
// 	
// 		GetGasAbilitySystemComponent()->EffectAssetTags.AddLambda(
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
// 	const ULevelUpInfo* LevelUpInfo = GetGasPlayerState()->LevelUpInfo;
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


AGasPlayerController* UOverlayWidgetController::GetGasPlayerController()
{
	if (GasPlayerController == nullptr)
	{
		GasPlayerController = Cast<AGasPlayerController>(PlayerController);
	}
	return GasPlayerController;
}

AGasPlayerState* UOverlayWidgetController::GetGasPlayerState()
{
	if (GasPlayerState == nullptr)
	{
		GasPlayerState = Cast<AGasPlayerState>(PlayerState);
	}
	return GasPlayerState;
}

UGasAbilitySystemComponent* UOverlayWidgetController::GetGasAbilitySystemComponent()
{
	if (GasAbilitySystemComponent == nullptr)
	{
		GasAbilitySystemComponent = Cast<UGasAbilitySystemComponent>(AbilitySystemComponent);
	}
	return GasAbilitySystemComponent;
}

UGasAttributeSet* UOverlayWidgetController::GetGasAttributeSet()
{
	if (GasAttributeSet == nullptr)
	{
		GasAttributeSet = Cast<UGasAttributeSet>(AttributeSet);
	}
	return GasAttributeSet;
}