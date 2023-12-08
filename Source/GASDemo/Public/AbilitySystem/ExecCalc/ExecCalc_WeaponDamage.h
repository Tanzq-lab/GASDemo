// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_WeaponDamage.generated.h"

/**
 * 
 */
UCLASS()
class GASDEMO_API UExecCalc_WeaponDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCalc_WeaponDamage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

protected:
	float HeadShotMultiplier;
};
