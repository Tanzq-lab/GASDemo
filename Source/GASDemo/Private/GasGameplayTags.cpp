// Copyright Amos Tan


#include "GasGameplayTags.h"

#include "GameplayTagsManager.h"

FGasGameplayTags FGasGameplayTags::GameplayTags;

void FGasGameplayTags::InitializeNativeGameplayTags()
{
	/*
	 * 首要属性
	 */
	
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString(TEXT("增加物理伤害"))
		);

	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString(TEXT("增加魔法伤害"))
		);

	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"),
		FString(TEXT("增加护甲和护甲穿透"))
		);

	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"),
		FString(TEXT("增加血量值"))
		);

	/*
	 * 次要属性
	 */
	
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString(TEXT("减少造成的伤害，提升格挡几率"))
		);

	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"),
		FString(TEXT("忽略一定比例的敌人护甲，并且增加暴击几率"))
		);

	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"),
		FString(TEXT("触发伤害减半的几率"))
		);

	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"),
		FString(TEXT("触发 双倍的伤害 + 加上暴击伤害 的几率"))
		);

	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"),
		FString(TEXT("当暴击伤害触发的时候产生的额外伤害"))
		);

	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"),
		FString(TEXT("减少敌人对自己造成伤害的暴击率"))
		);

	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"),
		FString(TEXT("每秒恢复血量的值"))
		);

	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"),
		FString(TEXT("每秒恢复蓝量的值"))
		);

	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString(TEXT("最大血量值"))
		);

	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"),
		FString(TEXT("最大魔法值"))
		);

	/*
	 * 武器相关属性
	 */
	GameplayTags.Weapon_Ammo_Rifle = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Weapon.Ammo.Rifle"),
			FString(TEXT("TODO"))
			);

	GameplayTags.Weapon_Ammo_Rocket = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Weapon.Ammo.Rocket"),
		FString(TEXT("TODO"))
		);

	GameplayTags.Weapon_Ammo_Shotgun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Weapon.Ammo.Shotgun"),
		FString(TEXT("TODO"))
		);

	GameplayTags.Weapon_Equipped_Rifle = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Als.OverlayMode.NonPistol.Rifle"),
			FString(TEXT("装配步枪"))
			);

	GameplayTags.Weapon_Equipped_RocketLauncher = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Als.OverlayMode.NonPistol.RocketLauncher"),
		FString(TEXT("装配火箭发射"))
		);

	GameplayTags.Weapon_Equipped_Shotgun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Als.OverlayMode.NonPistol.Shotgun"),
		FString(TEXT("装配散弹枪"))
		);


	
}
