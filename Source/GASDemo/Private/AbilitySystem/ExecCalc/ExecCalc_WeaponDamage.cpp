// Copyright Amos Tan


#include "AbilitySystem/ExecCalc/ExecCalc_WeaponDamage.h"

#include "GasGameplayTags.h"
#include "AbilitySystem/AttributeSet/GasAttributeSet.h"
#include "GASDemo/GASDemo.h"
#include "Utility/AlsConstants.h"

struct FDamageStatics
{
	DECLARE_ATTRIBUTE_DEF(Damage)
	DECLARE_ATTRIBUTE_DEF(Armor)

	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_DEF(UGasAttributeSet, Damage, Source, true)
		DEFINE_ATTRIBUTE_DEF(UGasAttributeSet, Armor, Source, true)
	}
};

static const FDamageStatics& DamageStatics()
{
	static FDamageStatics DStatics;
	return DStatics;
}

UExecCalc_WeaponDamage::UExecCalc_WeaponDamage()
{
	HeadShotMultiplier = 1.5f;

	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
}

void UExecCalc_WeaponDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayTagContainer AssetTags;
	Spec.GetAllAssetTags(AssetTags);

	// 从 source 和 target 中收集Tag，因为这会影响应该使用哪些 buff
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Armor = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, Armor);
	Armor = FMath::Max<float>(Armor, 0.0f);

	float Damage = 0.0f;
	// 捕获在 damage GE 上设置的可选damage值，作为 ExecutionCalculation 下的 CalculationModifier
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, Damage);
	// 如果存在，添加 SetByCaller damage
	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(GasDataTags::Damage, false, -1.0f), 0.0f);

	float UnmitigatedDamage = Damage;

	// 检测是否被爆头
	const FHitResult* Hit = Spec.GetContext().GetHitResult();
	if (AssetTags.HasTagExact(GasEffectDamageTags::CanHeadShot) && Hit && Hit->BoneName == UAlsConstants::HeadBoneName())
	{
		UnmitigatedDamage *= HeadShotMultiplier;
		FGameplayEffectSpec* MutableSpec = ExecutionParams.GetOwningSpecForPreExecuteMod();
		MutableSpec->AddDynamicAssetTag(GasEffectDamageTags::HeadShot);
	}

	// 返回对应的伤害值
	const float MitigatedDamage = (UnmitigatedDamage) * (100 / (100 + Armor));
	if (MitigatedDamage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(UGasAttributeSet::GetDamageAttribute(),
				EGameplayModOp::Additive, MitigatedDamage));
	}
}
