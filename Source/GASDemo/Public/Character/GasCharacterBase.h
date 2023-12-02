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

#pragma endregion

public:
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
protected:
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
};
