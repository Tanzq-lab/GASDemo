// Copyright TanZQ

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "GasPlayerState.generated.h"

class UAttributeSet;
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

public:
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// UPROPERTY(EditDefaultsOnly)
	// TObjectPtr<ULevelUpInfo> LevelUpInfo;
	//
	// FOnPlayerStatChanged OnXPChangedDelegate;
	// FOnLevelChanged OnLevelChangedDelegate;
	// FOnPlayerStatChanged OnAttributePointsChangedDelegate;
	// FOnPlayerStatChanged OnSpellPointsChangedDelegate;
	//
	// FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	// FORCEINLINE int32 GetXP() const { return XP; }
	// FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	// FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }
	//
	// void AddToXP(int32 InXP);
	// void AddToLevel(int32 InLevel);
	// void AddToAttributePoints(int32 InPoints);
	// void AddToSpellPoints(int32 InPoints);
	//
	// void SetXP(int32 InXP);
	// void SetLevel(int32 InLevel);
	// void SetAttributePoints(int32 InPoints);
	// void SetSpellPoints(int32 InPoints);
	
protected:

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

// private:
//
// 	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
// 	int32 Level = 1;
//
// 	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
// 	int32 XP = 0;
//
// 	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints)
// 	int32 AttributePoints = 0;
//
// 	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_SpellPoints)
// 	int32 SpellPoints = 0;
// 	
// 	UFUNCTION()
// 	void OnRep_Level(int32 OldLevel);
//
// 	UFUNCTION()
// 	void OnRep_XP(int32 OldXP);
//
// 	UFUNCTION()
// 	void OnRep_AttributePoints(int32 OldAttributePoints);
//
// 	UFUNCTION()
// 	void OnRep_SpellPoints(int32 OldSpellPoints);
};
