// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GasAT_WaitTargetDataUsingActor.generated.h"


class AGameplayAbilityTargetActor;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitTargetDataUsingActorDelegate, const FGameplayAbilityTargetDataHandle&,
                                            Data);

/**
 * 等待TargetData从一个已经产生的TargetActor和不销毁它时，它接收到的数据。
 * 最初的WaitTargetData注释希望我们大量子类化它，但是它的大多数函数都不是虚拟的。
 * 因此，这是一个完全重写它，以添加 bCreateKeyIfNotValidForMorePredicting 功能
 */
UCLASS()
class GASDEMO_API UGasAT_WaitTargetDataUsingActor : public UAbilityTask
{
	GENERATED_BODY()

public:
	UGasAT_WaitTargetDataUsingActor(const FObjectInitializer& ObjectInitializer);

private:
	UPROPERTY(BlueprintAssignable)
	FWaitTargetDataUsingActorDelegate ValidData;

	UPROPERTY(BlueprintAssignable)
	FWaitTargetDataUsingActorDelegate Cancelled;

	/**
	* Uses specified spawned TargetActor and waits for it to return valid data or to be canceled. The TargetActor is *NOT* destroyed.
	*
	* @param bCreateKeyIfNotValidForMorePredicting Will create a new scoped prediction key if the current scoped prediction key is not valid for more predicting.
	* If false, it will always create a new scoped prediction key. We would want to set this to true if we want to use a potentially existing valid scoped prediction
	* key like the ability's activation key in a batched ability.
	*/
	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", HideSpawnParms = "Instigator"), Category = "Ability|Tasks")
	static UGasAT_WaitTargetDataUsingActor* WaitTargetDataWithReusableActor(
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
