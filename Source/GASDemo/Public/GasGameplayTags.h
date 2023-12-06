// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * GasGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */

struct FGasGameplayTags
{
public:
    static const FGasGameplayTags& Get() { return GameplayTags;}
    static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;

	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

#pragma region 武器系统相关 Tag

	// 武器弹药类型
	FGameplayTag Weapon_Ammo_Rifle;
	FGameplayTag Weapon_Ammo_Rocket;
	FGameplayTag Weapon_Ammo_Shotgun;
	
#pragma endregion 

	
private:
    static FGasGameplayTags GameplayTags;
};
