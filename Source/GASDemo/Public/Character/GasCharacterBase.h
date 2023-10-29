// Copyright TanZQ

#pragma once

#include "AlsCharacter.h"
#include "GasCharacterBase.generated.h"

UCLASS()
class GASDEMO_API AGasCharacterBase : public AAlsCharacter
{
	GENERATED_BODY()
	
public:
	AGasCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
