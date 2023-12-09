// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "GasCharacterBase.h"
#include "Interaction/HighlightInterface.h"
#include "GasEnemy.generated.h"

class UGasAbilitySystemComponent;

UCLASS()
class GASDEMO_API AGasEnemy : public AGasCharacterBase, public IHighlightInterface
{
	GENERATED_BODY()

#pragma region init

public:
	AGasEnemy();
	
protected:
	UPROPERTY()
	TObjectPtr<UGasAbilitySystemComponent> GasAbilitySystemComponent;
	
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	
#pragma endregion
	
public:
	/** Highlight Interface */
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
};
