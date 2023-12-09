// Copyright Amos Tan


#include "Character/GasEnemy.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/GasAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/GasAttributeSet.h"
#include "GASDemo/GASDemo.h"


// Sets default values
AGasEnemy::AGasEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UGasAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UGasAttributeSet>("AttributeSet");

	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	GetMesh()->MarkRenderStateDirty();
}

void AGasEnemy::HighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(true);
	// Weapon->SetRenderCustomDepth(true);
}

void AGasEnemy::UnHighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(false);
	// Weapon->SetRenderCustomDepth(false);
}

void AGasEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
}

void AGasEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	GasAbilitySystemComponent = Cast<UGasAbilitySystemComponent>(AbilitySystemComponent);
	GasAbilitySystemComponent->AbilityActorInfoSet();
}
