// Copyright TanZQ

#pragma once

#include "AbilitySystemInterface.h"
#include "AlsCharacter.h"
#include "GasCharacterBase.generated.h"

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

	virtual void InitAbilityActorInfo();

#pragma endregion 
};
