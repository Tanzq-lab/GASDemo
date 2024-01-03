// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GasAbilitySystemLibrary.generated.h"

class AGasHUD;
struct FWidgetControllerParams;
class UWeaponWidgetController;
struct FGameplayAbilitySpecHandle;
class UGasGameplayAbility;
struct FGameplayAbilityTargetDataHandle;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class GASDEMO_API UGasAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	public:
	// Returns the player's editor window role - Server (listen host) or Client #
	UFUNCTION(BlueprintCallable)
	static FString GetPlayerEditorWindowRole(UWorld* World);


#pragma region GameplayAbility
	
	UFUNCTION(BlueprintCallable, Category = "GasAbilitySystemLibrary|Ability")
	static UGasGameplayAbility* GetPrimaryAbilityInstanceFromHandle(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle);
	
	UFUNCTION(BlueprintCallable, Category = "GasAbilitySystemLibrary|Ability")
	static UGasGameplayAbility* GetPrimaryAbilityInstanceFromClass(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayAbility> InAbilityClass);

	UFUNCTION(BlueprintCallable, Category = "GasAbilitySystemLibrary|Ability")
	static bool IsPrimaryAbilityInstanceActive(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle);

#pragma endregion 

	/**
	* FGameplayAbilitySpecHandle
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GasAbilitySystemLibrary|Ability")
	static bool IsAbilitySpecHandleValid(FGameplayAbilitySpecHandle Handle);


	/**
	* FGSGameplayEffectContainerSpec
	*/
	//
	// // USTRUCTs cannot contain UFUNCTIONS so we make static functions here
	// // Checks if spec has any effects
	// UFUNCTION(BlueprintPure, Category = "GasAbilitySystemLibrary|Container")
	// static bool DoesEffectContainerSpecHaveEffects(const FGSGameplayEffectContainerSpec& ContainerSpec);
	//
	// // Checks if spec has any targets
	// UFUNCTION(BlueprintPure, Category = "GasAbilitySystemLibrary|Container")
	// static bool DoesEffectContainerSpecHaveTargets(const FGSGameplayEffectContainerSpec& ContainerSpec);
	//
	// // Clears spec's targets
	// UFUNCTION(BlueprintCallable, Category = "GasAbilitySystemLibrary|Container")
	// static void ClearEffectContainerSpecTargets(UPARAM(ref) FGSGameplayEffectContainerSpec& ContainerSpec);
	//
	// // Adds targets to a copy of the passed in effect container spec and returns it
	// UFUNCTION(BlueprintCallable, Category = "GasAbilitySystemLibrary|Container", Meta = (AutoCreateRefTerm = "TargetData, HitResults, TargetActors"))
	// static void AddTargetsToEffectContainerSpec(UPARAM(ref) FGSGameplayEffectContainerSpec& ContainerSpec, const TArray<FGameplayAbilityTargetDataHandle>& TargetData, const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);
	//
	// // Applies container spec that was made from an ability
	// UFUNCTION(BlueprintCallable, Category = "GasAbilitySystemLibrary|Container")
	// static TArray<FActiveGameplayEffectHandle> ApplyExternalEffectContainerSpec(const FGSGameplayEffectContainerSpec& ContainerSpec);


	/**
	* FGSGameplayEffectContext
	*/

	// Returns TargetData
	// UFUNCTION(BlueprintCallable, Category = "GasAbilitySystemLibrary|EffectContext", Meta = (DisplayName = "GetTargetData"))
	// static FGameplayAbilityTargetDataHandle EffectContextGetTargetData(FGameplayEffectContextHandle EffectContext);

	// Adds TargetData
	// UFUNCTION(BlueprintCallable, Category = "GasAbilitySystemLibrary|EffectContext", Meta = (DisplayName = "AddTargetData"))
	// static void EffectContextAddTargetData(FGameplayEffectContextHandle EffectContextHandle, const FGameplayAbilityTargetDataHandle& TargetData);


	/**
	* FGameplayAbilityTargetDataHandle
	*/
	UFUNCTION(BlueprintCallable, Category = "GasAbilitySystemLibrary|TargetData")
	static void ClearTargetData(UPARAM(ref) FGameplayAbilityTargetDataHandle& TargetData);


	template<typename T>
	static T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
};


template <typename T>
T* UGasAbilitySystemLibrary::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
