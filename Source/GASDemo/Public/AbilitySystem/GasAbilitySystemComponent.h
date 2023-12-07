// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GasAbilitySystemComponent.generated.h"

struct FGasInputMapping;
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);

UCLASS()
class GASDEMO_API UGasAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

#pragma region 初始化相关逻辑
	
public:
	UGasAbilitySystemComponent();
	
	bool bCharacterAbilitiesGiven;
	
	FEffectAssetTags EffectAssetTags;
	
	void AbilityActorInfoSet();
	
protected:

	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

#pragma endregion 


#pragma region 输入相关模块

public:
	/**
	 * 通过 输入映射绑定 ASC 对应的 GA
	 */
	virtual void BindAbilityActivationToInputComponent(UInputComponent* InputComponent, TArray<FGasInputMapping>& InputMappings);

#pragma endregion 
};
