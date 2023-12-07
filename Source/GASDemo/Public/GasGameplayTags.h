// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

/**
 * GasGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */

namespace GasPrimaryAttributesTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Strength)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Intelligence)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Resilience)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Vigor)
}

namespace GasSecondaryAttributesTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_Armor)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_ArmorPenetration)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_BlockChance)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitChance)
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitDamage)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitResistance)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_HealthRegeneration)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_ManaRegeneration)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxHealth)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxMana)
}

namespace GasWeaponAmmoTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Weapon_Ammo_Rifle)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Weapon_Ammo_Rocket)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Weapon_Ammo_Shotgun)
}
