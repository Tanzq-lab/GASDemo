// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "GasGATA_Trace.h"
#include "GasGATA_LineTrace.generated.h"

/**
 * Reusable, configurable line trace TargetActor.
 * Meant to be used with GSAT_WaitTargetDataUsingActor instead of the default WaitTargetData AbilityTask as the default
 * one will destroy the TargetActor.
 */
UCLASS()
class GASDEMO_API AGasGATA_LineTrace : public AGasGATA_Trace
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGasGATA_LineTrace();

/**
	* 配置TargetActor以供使用。
	* 这个TargetActor可以在多种能力中使用，并且不能保证它将处于什么状态。
	* 需要确保一次只有一个能力在使用这个TargetActor。
	*
	* @param InStartLocation 射线检测的起始位置
	* @param InAimingTag 可选的。预测的瞄准GameplayTag。只有当我们在瞄准时修改扩散时才会使用。如果使用，也必须设置InAimingRemovalTag。
	* @param InAimingRemovalTag 可选的。瞄准移除的预测GameplayTag。只有当我们在瞄准时修改扩散时才会使用。如果使用，也必须设置InAimingTag。
	* @param InTraceProfile 射线检测的 Collision profile
	* @param InFilter Hit Actors必须通过这个过滤器才能在TargetData中返回。
	* @param InReticleClass 会出现在目标上方的瞄准镜。当目标被获取或丢失时，线将被生成/消失。
	* @param InReticleParams 瞄准镜的参数
	* @param bInIgnoreBlockingHits 忽略跟踪中的阻塞碰撞。如果你想穿墙瞄准，这很有用。
	* @param bInShouldProduceTargetDataOnServer
	* 如果设置了，除了客户端之外，这个TargetActor还会在服务器上产生TargetData， 客户端只会向服务器发送一个通用的“Confirm”事件。
	* 如果为false，客户端将把TargetData发送给服务器。这是由WaitTargetDataReusableActor AbilityTask处理的。
	* @param bInUsePersistentHitResults 搜索结果是否应该持续？ 点击确认/取消或当新的点击结果取代它们时，点击结果将被清除。
	* @param bInDebug 当为true时，这个TargetActor将显示跟踪和命中结果的调试射线。
	* @param bInTraceAffectsAimPitch 轨迹会影响瞄准距吗?
	* @param bInTraceFromPlayerViewPoint 我们是否应该从玩家视点而不是StartLocation进行跟踪？
	* TargetData HitResults将仍然具有TraceStart的StartLocation。
	* 这对于FPS来说很有用，因为我们想要从玩家的视点进行追踪，但从武器枪口处绘制子弹追踪。
	* TODO:AI控制器应该落回枪口位置。
	* @param bInUseAimingSpreadMod 我们是否应该根据目标来调整散布?如果为true，必须设置InAimingTag和InAimingRemovalTag。
	* @param InMaxRange 射线检测的最大距离。
	* @param InBaseSpread 扩散的基础值。
	* @param InAimingSpreadMod 可选的。如果瞄准，会修改扩散值。
	* @param InTargetingSpreadIncrement 每次增加扩散值的大小
	* @param InTargetingSpreadMax 扩散的最大值。
	* @param InMaxHitResultsPerTrace 跟踪可以返回的最大命中结果。 < 1 只返回跟踪结束点。
	* @param InNumberOfTraces 要执行的跟踪数。打算与 basspread 用于多射击武器，如霰弹枪。不打算与PersistentHitsResults一起使用。
	* 如果使用PersistentHitResults, NumberOfTraces被硬编码为1。
	*/
	UFUNCTION(BlueprintCallable)
	void Configure(
		UPARAM(DisplayName = "Start Location") const FGameplayAbilityTargetingLocationInfo& InStartLocation,
		UPARAM(DisplayName = "Aiming Tag") FGameplayTag InAimingTag,
		UPARAM(DisplayName = "Aiming Removal Tag") FGameplayTag InAimingRemovalTag,
		UPARAM(DisplayName = "Trace Profile") FCollisionProfileName InTraceProfile,
		UPARAM(DisplayName = "Filter") FGameplayTargetDataFilterHandle InFilter,
		UPARAM(DisplayName = "Reticle Class") TSubclassOf<AGameplayAbilityWorldReticle> InReticleClass,
		UPARAM(DisplayName = "Reticle Params") FWorldReticleParameters InReticleParams,
		UPARAM(DisplayName = "Ignore Blocking Hits") bool bInIgnoreBlockingHits = false,
		UPARAM(DisplayName = "Should Produce Target Data on Server") bool bInShouldProduceTargetDataOnServer = false,
		UPARAM(DisplayName = "Use Persistent Hit Results") bool bInUsePersistentHitResults = false,
		UPARAM(DisplayName = "Debug") bool bInDebug = false,
		UPARAM(DisplayName = "Trace Affects Aim Pitch") bool bInTraceAffectsAimPitch = true,
		UPARAM(DisplayName = "Trace From Player ViewPoint") bool bInTraceFromPlayerViewPoint = false,
		UPARAM(DisplayName = "Use Aiming Spread Mod") bool bInUseAimingSpreadMod = false,
		UPARAM(DisplayName = "Max Range") float InMaxRange = 999999.0f,
		UPARAM(DisplayName = "Base Targeting Spread") float InBaseSpread = 0.0f,
		UPARAM(DisplayName = "Aiming Spread Mod") float InAimingSpreadMod = 0.0f,
		UPARAM(DisplayName = "Targeting Spread Increment") float InTargetingSpreadIncrement = 0.0f,
		UPARAM(DisplayName = "Targeting Spread Max") float InTargetingSpreadMax = 0.0f,
		UPARAM(DisplayName = "Max Hit Results Per Trace") int32 InMaxHitResultsPerTrace = 1,
		UPARAM(DisplayName = "Number of Traces") int32 InNumberOfTraces = 1
	);

protected:

	virtual void DoTrace(TArray<FHitResult>& HitResults, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams Params) override;
	virtual void ShowDebugTrace(TArray<FHitResult>& HitResults, EDrawDebugTrace::Type DrawDebugType, float Duration = 2.0f) override;

#if ENABLE_DRAW_DEBUG
	// Util for drawing result of multi line trace from KismetTraceUtils.h
	void DrawDebugLineTraceMulti(const UWorld* World, const FVector& Start, const FVector& End, EDrawDebugTrace::Type DrawDebugType, bool bHit, const TArray<FHitResult>& OutHits, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime);
#endif // ENABLE_DRAW_DEBUG
};
