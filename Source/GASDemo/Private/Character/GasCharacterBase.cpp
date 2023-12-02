// Copyright Amos Tan


#include "Character/GasCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GASDemo/GASDemo.h"


AGasCharacterBase::AGasCharacterBase(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	// PrimaryActorTick.bCanEverTick = true;
	// const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	//
	// BurnDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("BurnDebuffComponent");
	// BurnDebuffComponent->SetupAttachment(GetRootComponent());
	// BurnDebuffComponent->DebuffTag = GameplayTags.Debuff_Burn;
	//
	// StunDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("StunDebuffComponent");
	// StunDebuffComponent->SetupAttachment(GetRootComponent());
	// StunDebuffComponent->DebuffTag = GameplayTags.Debuff_Stun;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	// Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	// Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	// Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//
	// EffectAttachComponent = CreateDefaultSubobject<USceneComponent>("EffectAttachPoint");
	// EffectAttachComponent->SetupAttachment(GetRootComponent());
	// HaloOfProtectionNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("HaloOfProtectionComponent");
	// HaloOfProtectionNiagaraComponent->SetupAttachment(EffectAttachComponent);
	// LifeSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("LifeSiphonNiagaraComponent");
	// LifeSiphonNiagaraComponent->SetupAttachment(EffectAttachComponent);
	// ManaSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("ManaSiphonNiagaraComponent");
	// ManaSiphonNiagaraComponent->SetupAttachment(EffectAttachComponent);

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

UAbilitySystemComponent* AGasCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGasCharacterBase::InitAbilityActorInfo()
{
}
