// Copyright Amos Tan


#include "Actors/GasWeapon.h"

#include "GasGameplayTags.h"
#include "AbilitySystem/GasAbilitySystemComponent.h"
#include "AbilitySystem/GasGameplayAbility.h"
#include "AbilitySystem/GameplayAbilityTargetActor/GasGATA_LineTrace.h"
#include "Character/GasCharacter.h"
#include "GASDemo/GASDemo.h"
#include "Net/UnrealNetwork.h"

AGasWeapon::AGasWeapon(): OwningCharacter(nullptr), AbilitySystemComponent(nullptr), PickupSound(nullptr),
                          ClipAmmo(0), MaxClipAmmo(0), 
                          LineTraceTargetActor(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	bNetUseOwnerRelevancy = true;
	NetUpdateFrequency = 100.0f;
	ClipAmmo = 0;
	MaxClipAmmo = 0;
	bInfiniteAmmo = false;
	AmmoType = GasWeaponAmmoTags::Weapon_Ammo_None;
	WeaponTag = AlsOverlayModeTags::Default;
	FireMode = GasWeaponFireModeTags::Weapon_FireMode_None;
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("WeaponMesh"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetupAttachment(RootComponent);
	WeaponMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
}

void AGasWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(AGasWeapon, OwningCharacter, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AGasWeapon, ClipAmmo, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AGasWeapon, MaxClipAmmo, COND_OwnerOnly);
}

void AGasWeapon::BeginPlay()
{
	ResetWeapon();
	
	Super::BeginPlay();
}

UAbilitySystemComponent* AGasWeapon::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGasWeapon::SetOwningCharacter(AGasCharacter* InOwningCharacter)
{
	OwningCharacter = InOwningCharacter;
	if (OwningCharacter)
	{
		AbilitySystemComponent = Cast<UGasAbilitySystemComponent>(OwningCharacter->GetAbilitySystemComponent());
		SetOwner(InOwningCharacter);
		// AttachToComponent(OwningCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}
	else
	{
		AbilitySystemComponent = nullptr;
		SetOwner(nullptr);
		// DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void AGasWeapon::AddAbilities()
{
	if (!IsValid(OwningCharacter) || !OwningCharacter->GetAbilitySystemComponent())
	{
		return;
	}

	UGasAbilitySystemComponent* ASC = Cast<UGasAbilitySystemComponent>(OwningCharacter->GetAbilitySystemComponent());

	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("%s %s Role: %d ASC is null"), *FString(__FUNCTION__), *GetName(), GET_ACTOR_ROLE_STRING(OwningCharacter));
		return;
	}

	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	for (TSubclassOf<UGasGameplayAbility>& Ability : Abilities)
	{
		AbilitySpecHandles.Add(ASC->GiveAbility(
			FGameplayAbilitySpec(Ability, GetAbilityLevel(Ability.GetDefaultObject()->AbilityID), static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this)));
	}
}

int32 AGasWeapon::GetAbilityLevel(EGasAbilityInputID AbilityID)
{
	// 默认为 1 
	return 1;
}

void AGasWeapon::Equip()
{
	if (!OwningCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("%s %s OwningCharacter is nullptr"), *FString(__FUNCTION__), *GetName());
		return;
	}
}

void AGasWeapon::UnEquip()
{
	if (OwningCharacter == nullptr)
	{
		return;
	}
}

USkeletalMeshComponent* AGasWeapon::GetWeaponMesh() const
{
	return WeaponMesh;
}

void AGasWeapon::OnOverlap(AActor* TargetActor)
{
	// 说明武器已经被拾取过了，不用执行后续逻辑。
	if (AbilitySystemComponent)
	{
		return;	
	}
	
	PickUpOnTouch(Cast<AGasCharacter>(TargetActor));
}

void AGasWeapon::PickUpOnTouch(AGasCharacter* InCharacter)
{
	if (!InCharacter)
	{
		return;
	}

	InCharacter->AddWeaponToInventory(this, true);
	RootComponent->SetVisibility(false, true);
}

USoundCue* AGasWeapon::GetPickupSound() const
{
	return PickupSound;
}

int32 AGasWeapon::GetClipAmmo() const
{
	return ClipAmmo;
}

int32 AGasWeapon::GetMaxClipAmmo() const
{
	return MaxClipAmmo;
}

void AGasWeapon::SetClipAmmo(int32 NewClipAmmo)
{
	const int32 OldClipAmmo = ClipAmmo;
	ClipAmmo = NewClipAmmo;
	OnClipAmmoChanged.Broadcast(OldClipAmmo, ClipAmmo);
}

void AGasWeapon::OnRep_ClipAmmo(int32 OldClipAmmo)
{
	OnClipAmmoChanged.Broadcast(OldClipAmmo, ClipAmmo);
}

void AGasWeapon::OnRep_MaxClipAmmo(int32 OldMaxClipAmmo)
{
	OnMaxClipAmmoChanged.Broadcast(OldMaxClipAmmo, MaxClipAmmo);
}

AGasGATA_LineTrace* AGasWeapon::GetLineTraceTargetActor()
{
	if (LineTraceTargetActor)
	{
		return LineTraceTargetActor;
	}

	LineTraceTargetActor = GetWorld()->SpawnActor<AGasGATA_LineTrace>();
	LineTraceTargetActor->SetOwner(this);
	return LineTraceTargetActor;
}

void AGasWeapon::ResetWeapon()
{
	FireMode = DefaultFireMode;
}

bool AGasWeapon::HasInfiniteAmmo() const
{
	return bInfiniteAmmo;
}

void AGasWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (LineTraceTargetActor)
	{
		LineTraceTargetActor->Destroy();
	}
	
	Super::EndPlay(EndPlayReason);
}

