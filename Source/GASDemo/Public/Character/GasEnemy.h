// Copyright TanZQ

#pragma once

#include "CoreMinimal.h"
#include "GasCharacterBase.h"
#include "Interaction/HighlightInterface.h"
#include "GasEnemy.generated.h"

UCLASS()
class GASDEMO_API AGasEnemy : public AGasCharacterBase, public IHighlightInterface
{
	GENERATED_BODY()

public:
	AGasEnemy();

	/** Highlight Interface */
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
};
