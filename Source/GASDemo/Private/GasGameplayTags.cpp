// Copyright Amos Tan


#include "GasGameplayTags.h"

#include "GameplayTagsManager.h"


namespace GasPrimaryAttributesTags
{
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Primary_Strength, FName("Attributes.Primary.Strength"))
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Primary_Intelligence, FName("Attributes.Primary.Intelligence"))
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Primary_Resilience, FName("Attributes.Primary.Resilience"))
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Primary_Vigor, FName("Attributes.Primary.Vigor"))
}

namespace GasSecondaryAttributesTags
{
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_Armor, FName("Attributes.Secondary.Armor"))
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_ArmorPenetration, FName("Attributes.Secondary.ArmorPenetration"))
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_BlockChance, FName("Attributes.Secondary.BlockChance"))
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_CriticalHitChance, FName("Attributes.Secondary.CriticalHitChance"))
	
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_CriticalHitDamage, FName("Attributes.Secondary.CriticalHitDamage"))
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_CriticalHitResistance, FName("Attributes.Secondary.CriticalHitResistance"))
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_HealthRegeneration, FName("Attributes.Secondary.HealthRegeneration"))
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_ManaRegeneration, FName("Attributes.Secondary.ManaRegeneration"))
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_MaxHealth, FName("Attributes.Secondary.MaxHealth"))
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_MaxMana, FName("Attributes.Secondary.MaxMana"))
}

namespace GasWeaponAmmoTags
{
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Ammo_Rifle, FName("Weapon.Ammo.Rifle"))
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Ammo_Rocket, FName("Weapon.Ammo.Rocket"))
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Ammo_Shotgun, FName("Weapon.Ammo.Shotgun"))
}