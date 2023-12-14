// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GasAT_WaitTargetDataReusableActor.generated.h"


class AGameplayAbilityTargetActor;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitTargetDataReusableActorDelegate, const FGameplayAbilityTargetDataHandle&,
                                            Data);

/**
 * 等待TargetData从一个已经产生的TargetActor和不销毁它时，它接收到的数据。
 * 最初的WaitTargetData注释希望我们大量子类化它，但是它的大多数函数都不是虚拟的。
 * 因此，这是一个完全重写它，以添加 bCreateKeyIfNotValidForMorePredicting 功能
 */
UCLASS()
class GASDEMO_API UGasAT_WaitTargetDataReusableActor : public UAbilityTask
{
	GENERATED_BODY()

public:
	UGasAT_WaitTargetDataReusableActor(const FObjectInitializer& ObjectInitializer);

private:
	UPROPERTY(BlueprintAssignable)
	FWaitTargetDataReusableActorDelegate ValidData;

	UPROPERTY(BlueprintAssignable)
	FWaitTargetDataReusableActorDelegate Cancelled;

	/**
	* Uses specified spawned TargetActor and waits for it to return valid data or to be canceled. The TargetActor is *NOT* destroyed.
	* 使用指定的TargetActor并等待它返回有效数据或被取消。TargetActor 并未销毁。
	*
	* @param bCreateKeyIfNotValidForMorePredicting 如果当前 scoped prediction key 对更多预测无效，将创建一个新的scoped prediction key。
	* 如果为false，它将始终创建一个新的作用域预测键。如果我们想要使用潜在存在的有效作用域预测键(如批处理能力中的能力激活键)，我们将希望将此设置为true。
	*/
	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", HideSpawnParms = "Instigator"), Category = "Ability|Tasks")
	static UGasAT_WaitTargetDataReusableActor* WaitTargetDataWithReusableActor(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		TEnumAsByte<EGameplayTargetingConfirmation::Type> ConfirmationType,
		AGameplayAbilityTargetActor* InTargetActor,
		bool bCreateKeyIfNotValidForMorePredicting = false
	);

	virtual void Activate() override;

	UFUNCTION()
	virtual void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& Data, FGameplayTag ActivationTag);

	UFUNCTION()
	virtual void OnTargetDataReplicatedCancelledCallback();

	UFUNCTION()
	virtual void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION()
	virtual void OnTargetDataCancelledCallback(const FGameplayAbilityTargetDataHandle& Data);

	// Called when the ability is asked to confirm from an outside node. What this means depends on the individual task. By default, this does nothing other than ending if bEndTask is true.
	virtual void ExternalConfirm(bool bEndTask) override;

	// Called when the ability is asked to cancel from an outside node. What this means depends on the individual task. By default, this does nothing other than ending the task.
	virtual void ExternalCancel() override;

protected:
	UPROPERTY()
	AGameplayAbilityTargetActor* TargetActor;

	bool bCreateKeyIfNotValidForMorePredicting;

	TEnumAsByte<EGameplayTargetingConfirmation::Type> ConfirmationType;

	FDelegateHandle OnTargetDataReplicatedCallbackDelegateHandle;

	virtual void InitializeTargetActor() const;
	virtual void FinalizeTargetActor() const;

	virtual void RegisterTargetDataCallbacks();

	virtual void OnDestroy(bool AbilityEnded) override;

	virtual bool ShouldReplicateDataToServer() const;
};
