// Copyright Amos Tan

#pragma once

#include "AbilitySystemInterface.h"
#include "AlsCharacter.h"
#include "GasCharacterBase.generated.h"

class UGameplayEffect;
class UAttributeSet;

UCLASS()
class GASDEMO_API AGasCharacterBase : public AAlsCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
#pragma region 初始化相关逻辑
public:
	AGasCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// 只能在服务器上调用， 服务器上将 GA 删除了， 会同步给各个客户端。
	virtual void RemoveCharacterAbilities();

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;

	virtual void InitAbilityActorInfo();

	virtual void AddCharacterAbilities();

#pragma endregion

public:
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
protected:
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
};
