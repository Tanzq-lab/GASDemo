// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "GasPlayerState.generated.h"

class UGasAmmoAttributeSet;
class UGasAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /*StatValue*/)

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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
#pragma endregion

public:
	UGasAttributeSet* GetGasAttributeSet() const { return GasAttributeSet; }
	
	UGasAmmoAttributeSet* GetGasAmmoAttributeSet() const { return GasAmmoAttributeSet; }

	FOnPlayerStatChanged OnAttributePointsChangedDelegate;

	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	
protected:

	UPROPERTY()
	TObjectPtr<UGasAttributeSet> GasAttributeSet;

	UPROPERTY()
	TObjectPtr<UGasAmmoAttributeSet> GasAmmoAttributeSet;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints = 0;

	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);
};
