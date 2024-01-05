// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GasAbilitySystemComponent.generated.h"

struct FGasInputMapping;
class USkeletalMeshComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);

/**
* Data about montages that were played locally (all montages in case of server. predictive montages in case of client). Never replicated directly.
*/
USTRUCT()
struct GASDEMO_API FGameplayAbilityLocalAnimMontageForMesh
{
	GENERATED_BODY();

public:
	UPROPERTY()
	USkeletalMeshComponent* Mesh;

	UPROPERTY()
	FGameplayAbilityLocalAnimMontage LocalMontageInfo;

	FGameplayAbilityLocalAnimMontageForMesh() : Mesh(nullptr), LocalMontageInfo()
	{
	}

	FGameplayAbilityLocalAnimMontageForMesh(USkeletalMeshComponent* InMesh)
		: Mesh(InMesh), LocalMontageInfo()
	{
	}

	FGameplayAbilityLocalAnimMontageForMesh(USkeletalMeshComponent* InMesh, FGameplayAbilityLocalAnimMontage& InLocalMontageInfo)
		: Mesh(InMesh), LocalMontageInfo(InLocalMontageInfo)
	{
	}
};

/**
* Data about montages that is replicated to simulated clients.
*/
USTRUCT()
struct GASDEMO_API FGameplayAbilityRepAnimMontageForMesh
{
	GENERATED_BODY();

public:
	UPROPERTY()
	USkeletalMeshComponent* Mesh;

	UPROPERTY()
	FGameplayAbilityRepAnimMontage RepMontageInfo;

	FGameplayAbilityRepAnimMontageForMesh() : Mesh(nullptr), RepMontageInfo()
	{
	}

	FGameplayAbilityRepAnimMontageForMesh(USkeletalMeshComponent* InMesh)
		: Mesh(InMesh), RepMontageInfo()
	{
	}
};



UCLASS()
class GASDEMO_API UGasAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

#pragma region 初始化相关逻辑
	
public:
	UGasAbilitySystemComponent();
	
	bool bCharacterAbilitiesGiven;
	
	FEffectAssetTags EffectAssetTags;
	
	void AbilityActorInfoSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:

	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

#pragma endregion 


#pragma region 输入相关模块

public:
	/**
	 * 通过 输入映射绑定 ASC 对应的 GA
	 */
	virtual void BindAbilityToInputComponent(UInputComponent* InputComponent, TArray<FGasInputMapping>& InputMappings);

#pragma endregion


public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Abilities")
	FGameplayAbilitySpecHandle FindAbilitySpecHandleForClass(TSubclassOf<UGameplayAbility> AbilityClass, UObject* OptionalSourceObject=nullptr);

	// 启用服务器批处理RPC，ASC默认为关闭。
	virtual bool ShouldDoServerAbilityRPCBatch() const override { return true; }
	
	/**
	 * 尝试激活给定的能力句柄并将所有rpc批处理为一个。
	 * 这将只批处理在一帧中发生的所有rpc。
	 * 最好的情况是：我们将ActivateAbility、SendTargetData和EndAbility批处理到一个RPC中，而不是三个。
	 * 最坏的情况是：我们将ActivateAbility和SendTargetData批处理到一个RPC中，而不是两个，然后单独调用EndAbility RPC。
	 * 如果我们不能批处理SendTargetData或EndAbility与ActivateAbility，因
	 * 为它们不会发生在同一帧中，例如，由于潜在的能力任务，然后批处理没有帮助，我们应该正常激活。
	 * 单发射击(半自动射击)将ActivateAbility、SendTargetData和EndAbility合并到一个RPC中，而不是三个。
	 * 全自动射击将ActivateAbility和SendTargetData合并为一个RPC，而不是第一颗子弹的两个RPC。
	 * 每个后续项目符号都是SendTargetData的一个RPC。
	 * 然后，当我们完成发射时，我们为EndAbility发送最后一个RPC。
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual bool BatchRPCTryActivateAbility(FGameplayAbilitySpecHandle InAbilityHandle, bool EndAbilityImmediately);
	
	// Version of function in AbilitySystemGlobals that returns correct type
	static UGasAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);


#pragma region 播放动画相关

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// Copy LocalAnimMontageInfo into RepAnimMontageInfo
	void AnimMontage_UpdateReplicatedDataForMesh(USkeletalMeshComponent* InMesh);
	void AnimMontage_UpdateReplicatedDataForMesh(FGameplayAbilityRepAnimMontageForMesh& OutRepAnimMontageInfo);
	
	
	// Plays a montage and handles replication and prediction based on passed in ability/activation info
	virtual float PlayMontageForMesh(UGameplayAbility* AnimatingAbility, class USkeletalMeshComponent* InMesh, FGameplayAbilityActivationInfo ActivationInfo, UAnimMontage* Montage, float InPlayRate, FName StartSectionName = NAME_None, bool bReplicateMontage = true);

	// Finds the existing FGameplayAbilityLocalAnimMontageForMesh for the mesh or creates one if it doesn't exist
	FGameplayAbilityLocalAnimMontageForMesh& GetLocalAnimMontageInfoForMesh(USkeletalMeshComponent* InMesh);
	// Finds the existing FGameplayAbilityRepAnimMontageForMesh for the mesh or creates one if it doesn't exist
	FGameplayAbilityRepAnimMontageForMesh& GetGameplayAbilityRepAnimMontageForMesh(USkeletalMeshComponent* InMesh);

	// Plays a montage without updating replication/prediction structures. Used by simulated proxies when replication tells them to play a montage.
	virtual float PlayMontageSimulatedForMesh(USkeletalMeshComponent* InMesh, UAnimMontage* Montage, float InPlayRate, FName StartSectionName = NAME_None);

	// Stops whatever montage is currently playing. Expectation is caller should only be stopping it if they are the current animating ability (or have good reason not to check)
	virtual void CurrentMontageStopForMesh(USkeletalMeshComponent* InMesh, float OverrideBlendOutTime = -1.0f);
	
	// Returns the current animating ability
	UGameplayAbility* GetAnimatingAbilityFromAnyMesh();

	// Returns the montage that is playing for the mesh
	UAnimMontage* GetCurrentMontageForMesh(USkeletalMeshComponent* InMesh);
	
protected:
	
	// Data structure for montages that were instigated locally (everything if server, predictive if client. replicated if simulated proxy)
	// Will be max one element per skeletal mesh on the AvatarActor
	UPROPERTY()
	TArray<FGameplayAbilityLocalAnimMontageForMesh> LocalAnimMontageInfoForMeshes;

	// Data structure for replicating montage info to simulated clients
	// Will be max one element per skeletal mesh on the AvatarActor
	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedAnimMontageForMesh)
	TArray<FGameplayAbilityRepAnimMontageForMesh> RepAnimMontageInfoForMeshes;

	UFUNCTION()
	virtual void OnRep_ReplicatedAnimMontageForMesh();

	// Returns true if we are ready to handle replicated montage information
	virtual bool IsReadyForReplicatedMontageForMesh();
	
	// Called when a prediction key that played a montage is rejected
	void OnPredictiveMontageRejectedForMesh(USkeletalMeshComponent* InMesh, UAnimMontage* PredictiveMontage);
	
#pragma endregion

public:
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);
	
};
