// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GASDemo/GASDemo.h"
#include "Utility/GasAbilityTypes.h"
#include "GasGameplayAbility.generated.h"

class USkeletalMeshComponent;
class UAnimMontage;

USTRUCT()
struct GASDEMO_API FAbilityMeshMontage
{
	GENERATED_BODY()

public:
	UPROPERTY()
	USkeletalMeshComponent* Mesh;

	UPROPERTY()
	UAnimMontage* Montage;

	FAbilityMeshMontage() : Mesh(nullptr), Montage(nullptr)
	{
	}

	FAbilityMeshMontage(class USkeletalMeshComponent* InMesh, class UAnimMontage* InMontage) 
		: Mesh(InMesh), Montage(InMontage)
	{
	}
};

/**
 * 
 */
UCLASS()
class GASDEMO_API UGasGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:

	UGasGameplayAbility();

	
public:
	// 当按下输入键时，此设置的技能将自动激活
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGasAbilityInputID AbilityInputID = EGasAbilityInputID::None;

	// 值将能力与插槽关联起来，而不将其绑定到自动激活的输入。
	// 被动技能不会与输入绑定，所以我们需要一种将技能与插槽联系起来的方法。
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGasAbilityInputID AbilityID = EGasAbilityInputID::None;

	// 尝试激活给定的能力句柄并将所有rpc批处理为一个。
	// 这将只批处理在一帧中发生的所有rpc。
	// 最好的情况是，我们将ActivateAbility、SendTargetData和EndAbility批处理到一个RPC中，而不是三个。
	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual bool BatchRPCTryActivateAbility(FGameplayAbilitySpecHandle InAbilityHandle, bool EndAbilityImmediately);

	// 与 K2_EndAbility 函数相同的作用. 用与批处理，通过外部的的方式关闭 GA
	virtual void ExternalEndAbility();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameplayEffects")
	TMap<FGameplayTag, FGasGameplayEffectContainer> EffectContainerMap;

	// 从EffectContainerMap中搜索并制作一个稍后应用的 gameplay effect container spec
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual FGasGameplayEffectContainerSpec MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	// Make gameplay effect container spec to be applied later, using the passed in container
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual FGasGameplayEffectContainerSpec MakeEffectContainerSpecFromContainer(const FGasGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	// Expose GetSourceObject to Blueprint. Retrieves the SourceObject associated with this ability. Callable on non instanced abilities.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability", meta = (DisplayName = "Get Source Object"))
	UObject* K2_GetSourceObject(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo) const;


	// Applies a gameplay effect container spec that was previously created
	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainerSpec(const FGasGameplayEffectContainerSpec& ContainerSpec);

#pragma region 动画相关

public:
	/** Call to set/get the current montage from a montage task. Set to allow hooking up montage events to ability events */
	virtual void SetCurrentMontageForMesh(USkeletalMeshComponent* InMesh, class UAnimMontage* InCurrentMontage);


protected:

	/** Active montages being played by this ability */
	UPROPERTY()
	TArray<FAbilityMeshMontage> CurrentAbilityMeshMontages;
	
	bool FindAbilityMeshMontage(USkeletalMeshComponent* InMesh, FAbilityMeshMontage& InAbilityMontage);
	
#pragma endregion 

#pragma region cost

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	// Allows C++ and Blueprint abilities to override how cost is checked in case they don't use a GE like weapon ammo
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability")
	bool GasCheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo) const;

	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	// Allows C++ and Blueprint abilities to override how cost is applied in case they don't use a GE like weapon ammo
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability")
	void GasApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const;

	
#pragma endregion 

#pragma region Debug

	// 返回当前预测键以及它是否对更多预测有效。用于调试能力预测窗口。
	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual FString GetCurrentPredictionKeyStatus();

#pragma endregion 
};
