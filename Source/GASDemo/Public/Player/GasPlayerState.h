// Copyright TanZQ

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "GasPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GASDEMO_API AGasPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
#pragma region 初始化相关逻辑
public:
	AGasPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
#pragma endregion 
};
