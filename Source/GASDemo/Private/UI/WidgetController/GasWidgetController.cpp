// Copyright Amos Tan


#include "UI/WidgetController/GasWidgetController.h"

#include "AbilitySystem/GasAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/GasAttributeSet.h"
#include "Player/GasPlayerController.h"
#include "Player/GasPlayerState.h"

void UGasWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UGasWidgetController::BroadcastInitialValues()
{
	
}

void UGasWidgetController::BindCallbacksToDependencies()
{
	
}

void UGasWidgetController::BroadcastAbilityInfo()
{
	// if (!GetGasASC()->bStartupAbilitiesGiven) return;
	//
	// FForEachAbility BroadcastDelegate;
	// BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	// {
	// 	FGasAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(GasAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
	// 	Info.InputTag = GasAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
	// 	Info.StatusTag = GasAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
	// 	AbilityInfoDelegate.Broadcast(Info);
	// });
	// GetGasASC()->ForEachAbility(BroadcastDelegate);
}