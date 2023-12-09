// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GasPlayerController.generated.h"

struct FGameplayTagContainer;
class AGasCharacterBase;
class UGasAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class GASDEMO_API AGasPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AGasPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

private:

	UPROPERTY()
	TObjectPtr<UGasAbilitySystemComponent> GasAbilitySystemComponent;

	UGasAbilitySystemComponent* GetASC();

#pragma region Actor 高亮
private:
	UPROPERTY()
	TObjectPtr<AActor> LastActor;
	
	UPROPERTY()
	TObjectPtr<AActor> ThisActor;
	
	FHitResult ReticleHit;
	
	static void HighlightActor(AActor* InActor);
	static void UnHighlightActor(AActor* InActor);
	void ReticleTrace();

#pragma endregion

#pragma region 武器系统

	// UI 相关 处理逻辑

	
#pragma endregion


#pragma region UI

public:
	UFUNCTION(Client, Reliable, WithValidation)
	void ShowDamageNumber(float DamageAmount, AGasCharacterBase* TargetCharacter, FGameplayTagContainer DamageNumberTags);
	
#pragma endregion 
};
