// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GasInputMappingContext.generated.h"

enum class EGasAbilityInputID : uint8;
class UGasGameplayAbility;

USTRUCT(BlueprintType)
struct GASDEMO_API FGasInputMapping
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Input")
	EGasAbilityInputID InputID;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Input")
	TArray<FKey> Keys;
};


/**
 * 
 */
UCLASS(BlueprintType, config = Input)
class GASDEMO_API UGasInputMappingContext : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Mappings")
	TArray<TSubclassOf<UGasGameplayAbility>> Abilities;

	UPROPERTY(config, BlueprintReadOnly, EditAnywhere, Category = "Mappings")
	TArray<FGasInputMapping> Mappings;
	
};
