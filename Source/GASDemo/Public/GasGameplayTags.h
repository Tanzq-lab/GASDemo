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
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Strength)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Intelligence)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Resilience)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Vigor)
}

namespace GasSecondaryAttributesTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Armor)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ArmorPenetration)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BlockChance)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CriticalHitChance)
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CriticalHitDamage)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CriticalHitResistance)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(HealthRegeneration)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ManaRegeneration)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxHealth)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxMana)
}

namespace GasWeaponAmmoTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(None)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Rifle)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Rocket)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shotgun)
}

namespace GasWeaponFireModeTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(None)

	// Rifle
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Rifle_Aiming)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Rifle_AimingRemoval)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Rifle_FireMode_Burst)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Rifle_FireMode_FullAuto)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Rifle_FireMode_SemiAuto)
}

namespace GasFailTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Weapon_Activation_OnCooldown)
}

namespace GasDataTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage)
}

namespace GasEffectDamageTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CanHeadShot)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(HeadShot)
}

namespace GasGameplayCueTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Weapon_Rifle_Fire)
}

namespace GasAbilityTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Weapon)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Weapon_Reload)
}