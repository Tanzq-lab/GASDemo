// Copyright Amos Tan


#include "GasGameplayTags.h"

#include "GameplayTagsManager.h"


namespace GasPrimaryAttributesTags
{
	UE_DEFINE_GAMEPLAY_TAG(Strength, FName{TEXTVIEW("Attributes.Primary.Strength")})
	UE_DEFINE_GAMEPLAY_TAG(Intelligence, FName{TEXTVIEW("Attributes.Primary.Intelligence")})
	UE_DEFINE_GAMEPLAY_TAG(Resilience, FName{TEXTVIEW("Attributes.Primary.Resilience")})
	UE_DEFINE_GAMEPLAY_TAG(Vigor, FName{TEXTVIEW("Attributes.Primary.Vigor")})
}

namespace GasSecondaryAttributesTags
{
	UE_DEFINE_GAMEPLAY_TAG(Armor, FName{TEXTVIEW("Attributes.Secondary.Armor")})
	UE_DEFINE_GAMEPLAY_TAG(ArmorPenetration, FName{TEXTVIEW("Attributes.Secondary.ArmorPenetration")})
	UE_DEFINE_GAMEPLAY_TAG(BlockChance, FName{TEXTVIEW("Attributes.Secondary.BlockChance")})
	UE_DEFINE_GAMEPLAY_TAG(CriticalHitChance, FName{TEXTVIEW("Attributes.Secondary.CriticalHitChance")})
	
	UE_DEFINE_GAMEPLAY_TAG(CriticalHitDamage, FName{TEXTVIEW("Attributes.Secondary.CriticalHitDamage")})
	UE_DEFINE_GAMEPLAY_TAG(CriticalHitResistance, FName{TEXTVIEW("Attributes.Secondary.CriticalHitResistance")})
	UE_DEFINE_GAMEPLAY_TAG(HealthRegeneration, FName{TEXTVIEW("Attributes.Secondary.HealthRegeneration")})
	UE_DEFINE_GAMEPLAY_TAG(ManaRegeneration, FName{TEXTVIEW("Attributes.Secondary.ManaRegeneration")})
	UE_DEFINE_GAMEPLAY_TAG(MaxHealth, FName{TEXTVIEW("Attributes.Secondary.MaxHealth")})
	UE_DEFINE_GAMEPLAY_TAG(MaxMana, FName{TEXTVIEW("Attributes.Secondary.MaxMana")})
}

namespace GasWeaponAmmoTags
{
	UE_DEFINE_GAMEPLAY_TAG(None, FName{TEXTVIEW("Weapon.Ammo.None")})
	UE_DEFINE_GAMEPLAY_TAG(Rifle, FName{TEXTVIEW("Weapon.Ammo.Rifle")})
	UE_DEFINE_GAMEPLAY_TAG(Rocket, FName{TEXTVIEW("Weapon.Ammo.Rocket")})
	UE_DEFINE_GAMEPLAY_TAG(Shotgun, FName{TEXTVIEW("Weapon.Ammo.Shotgun")})
}

namespace GasWeaponFireModeTags
{
	UE_DEFINE_GAMEPLAY_TAG(None, FName{TEXTVIEW("Weapon.FireMode.None")})

	// Rifle
	UE_DEFINE_GAMEPLAY_TAG(Rifle_Aiming, FName{TEXTVIEW("Weapon.Rifle.Aiming")})
	UE_DEFINE_GAMEPLAY_TAG(Rifle_AimingRemoval, FName{TEXTVIEW("Weapon.Rifle.AimingRemoval")})
	UE_DEFINE_GAMEPLAY_TAG(Rifle_FireMode_Burst, FName{TEXTVIEW("Weapon.Rifle.FireMode.Burst")})
	UE_DEFINE_GAMEPLAY_TAG(Rifle_FireMode_FullAuto, FName{TEXTVIEW("Weapon.Rifle.FireMode.FullAuto")})
	UE_DEFINE_GAMEPLAY_TAG(Rifle_FireMode_SemiAuto, FName{TEXTVIEW("Weapon.Rifle.FireMode.SemiAuto")})
}

namespace GasFailTags
{
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Activation_OnCooldown, FName{TEXTVIEW("Fail.Weapon.Activation.OnCooldown")})
}

namespace GasDataTags
{
	UE_DEFINE_GAMEPLAY_TAG(Damage, FName{TEXTVIEW("Data.Damage")} )
}

namespace GasEffectDamageTags
{
	UE_DEFINE_GAMEPLAY_TAG(CanHeadShot, FName{TEXTVIEW("Effect.Damage.CanHeadShot")} )
	UE_DEFINE_GAMEPLAY_TAG(HeadShot, FName{TEXTVIEW("Effect.Damage.HeadShot")} )
}

namespace GasGameplayCueTags
{
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Rifle_Fire, FName{TEXTVIEW("GameplayCue.Weapon.Rifle.Fire")} )
}

namespace GasAbilityTags
{
	UE_DEFINE_GAMEPLAY_TAG(Weapon, FName{TEXTVIEW("Abilities.Weapon")} )
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Reload, FName{TEXTVIEW("Abilities.Weapon.Reload")} )
}

