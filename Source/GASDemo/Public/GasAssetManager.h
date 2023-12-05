// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GasAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class GASDEMO_API UGasAssetManager : public UAssetManager
{
	GENERATED_BODY()

protected:

	virtual void StartInitialLoading() override;
};
