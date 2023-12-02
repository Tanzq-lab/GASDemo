// Copyright Amos Tan


#include "Character/GasEnemy.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/GasAbilitySystemComponent.h"
#include "GASDemo/GASDemo.h"


// Sets default values
AGasEnemy::AGasEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UGasAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	// AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	//
	// HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	// HealthBar->SetupAttachment(GetRootComponent());

	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	GetMesh()->MarkRenderStateDirty();
	// Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	// Weapon->MarkRenderStateDirty();
	//
	// BaseWalkSpeed = 250.f;
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
