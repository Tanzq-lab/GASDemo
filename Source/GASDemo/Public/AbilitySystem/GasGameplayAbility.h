// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GASDemo/GASDemo.h"
#include "GasGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GASDEMO_API UGasGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	// Abilities with this set will automatically activate when the input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGasAbilityInputID AbilityInputID = EGasAbilityInputID::None;

	// Value to associate an ability with an slot without tying it to an automatically activated input.
	// Passive abilities won't be tied to an input so we need a way to generically associate abilities with slots.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGasAbilityInputID AbilityID = EGasAbilityInputID::None;
};

