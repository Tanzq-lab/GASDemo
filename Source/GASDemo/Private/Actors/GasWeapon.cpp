// Copyright Amos Tan


#include "Actors/GasWeapon.h"

#include "GasGameplayTags.h"
#include "AbilitySystem/GasAbilitySystemComponent.h"
#include "AbilitySystem/GasGameplayAbility.h"
#include "AbilitySystem/GameplayAbilityTargetActor/GasGATA_LineTrace.h"
#include "Character/GasCharacter.h"
#include "GASDemo/GASDemo.h"
#include "Net/UnrealNetwork.h"

AGasWeapon::AGasWeapon(): OwningCharacter(nullptr), GasAbilitySystemComponent(nullptr), PickupSound(nullptr),
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
	AmmoType = GasWeaponAmmoTags::None;
	WeaponTag = AlsOverlayModeTags::Default;
	FireMode = GasWeaponFireModeTags::None;
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("WeaponMesh"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(WeaponMesh);
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
	return GasAbilitySystemComponent;
}

void AGasWeapon::SetOwningCharacter(AGasCharacter* InOwningCharacter)
{
	OwningCharacter = InOwningCharacter;
	if (OwningCharacter)
	{
		GasAbilitySystemComponent = Cast<UGasAbilitySystemComponent>(OwningCharacter->GetAbilitySystemComponent());
		checkf(GasAbilitySystemComponent, TEXT("%s %s Role: %d ASC is null"), *FString(__FUNCTION__), *GetName(), GET_ACTOR_ROLE_STRING(OwningCharacter));
		SetOwner(InOwningCharacter);
		RootComponent->SetVisibility(false, true);
	}
	else
	{
		GasAbilitySystemComponent = nullptr;
		SetOwner(nullptr);
	}
}

void AGasWeapon::AddAbilities()
{
	if (!IsValid(OwningCharacter) || !IsValid(GasAbilitySystemComponent))
	{
		return;
	}

	// 只在服务器上赋予GA
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	for (TSubclassOf<UGasGameplayAbility>& Ability : Abilities)
	{
		AbilitySpecHandles.Add(GasAbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(
			Ability, GetAbilityLevel(Ability.GetDefaultObject()->AbilityID),
			static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this)));
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

	// TODO 执行 Attach相关逻辑
}

void AGasWeapon::UnEquip()
{
	if (OwningCharacter == nullptr)
	{
		return;
	}

	// 该武器将不是CurrentWeapon了的时候执行该函数，执行该函数不代表该武器被抛弃。
	// TODO 执行 Detach 相关的逻辑 
}

USkeletalMeshComponent* AGasWeapon::GetWeaponMesh() const
{
	return WeaponMesh;
}

void AGasWeapon::OnOverlap(AActor* TargetActor)
{
	// 说明武器已经被拾取过了，不用执行后续逻辑。
	if (GasAbilitySystemComponent)
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

