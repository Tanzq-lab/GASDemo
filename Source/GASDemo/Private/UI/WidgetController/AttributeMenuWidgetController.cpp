// Copyright Amos Tan


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/GasAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/GasAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/GasPlayerController.h"
#include "Player/GasPlayerState.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	check(AttributeInfo);
	for (auto& Pair : GetGasAttributeSet()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
		[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);
	UGasAttributeSet* AS = GetGasAttributeSet();

	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
                                                            const FGameplayAttribute& Attribute) const
{
	FGasAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

AGasPlayerController* UAttributeMenuWidgetController::GetGasPlayerController()
{
	if (GasPlayerController == nullptr)
	{
		GasPlayerController = CastChecked<AGasPlayerController>(PlayerController);
	}
	return GasPlayerController;
}

AGasPlayerState* UAttributeMenuWidgetController::GetGasPlayerState()
{
	if (GasPlayerState == nullptr)
	{
		GasPlayerState = CastChecked<AGasPlayerState>(PlayerState);
	}
	return GasPlayerState;
}

UGasAbilitySystemComponent* UAttributeMenuWidgetController::GetGasAbilitySystemComponent()
{
	if (GasAbilitySystemComponent == nullptr)
	{
		GasAbilitySystemComponent = CastChecked<UGasAbilitySystemComponent>(AbilitySystemComponent);
	}
	return GasAbilitySystemComponent;
}

UGasAttributeSet* UAttributeMenuWidgetController::GetGasAttributeSet()
{
	if (GasAttributeSet == nullptr)
	{
		GasAttributeSet = CastChecked<UGasAttributeSet>(AttributeSet);
	}
	return GasAttributeSet;
}
