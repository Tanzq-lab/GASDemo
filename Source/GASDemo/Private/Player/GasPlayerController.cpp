// Copyright Amos Tan


#include "Player/GasPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/GasAbilitySystemComponent.h"
#include "Character/GasCharacterBase.h"
#include "GASDemo/GASDemo.h"
#include "Interaction/HighlightInterface.h"
#include "Kismet/KismetSystemLibrary.h"

AGasPlayerController::AGasPlayerController()
{
	bReplicates = true;
}

void AGasPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	// TODO  后续提取成一个 Ability
	ReticleTrace();
}

UGasAbilitySystemComponent* AGasPlayerController::GetASC()
{
	if (GasAbilitySystemComponent == nullptr)
	{
		GasAbilitySystemComponent = Cast<UGasAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return GasAbilitySystemComponent;
}

void AGasPlayerController::HighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighlightInterface>())
	{
		IHighlightInterface::Execute_HighlightActor(InActor);
	}
}

void AGasPlayerController::UnHighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighlightInterface>())
	{
		IHighlightInterface::Execute_UnHighlightActor(InActor);
	}
}

void AGasPlayerController::ReticleTrace()
{
	// if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_CursorTrace))
	// {
	// 	UnHighlightActor(LastActor);
	// 	UnHighlightActor(ThisActor);
	// 	if (IsValid(ThisActor) && ThisActor->Implements<UHighlightInterface>())
	//
	// 		LastActor = nullptr;
	// 	ThisActor = nullptr;
	// 	return;
	// }
	const ECollisionChannel TraceChannel = /*IsValid(MagicCircle) ? ECC_ExcludePlayers : */ECC_Visibility;
	auto World = GetWorld();
	check(World);
	if (UGameViewportClient* ViewportClient = World->GetGameViewport())
	{
		FVector2D OutSize;
		ViewportClient->GetViewportSize(OutSize);
		GetHitResultAtScreenPosition(OutSize / 2.f, TraceChannel, false, ReticleHit);
	}
	
	if (!ReticleHit.bBlockingHit) return;

	LastActor = ThisActor;
	if (IsValid(ReticleHit.GetActor()) && ReticleHit.GetActor()->Implements<UHighlightInterface>())
	{
		ThisActor = ReticleHit.GetActor();
	}
	else
	{
		ThisActor = nullptr;
	}

	if (LastActor != ThisActor)
	{
		UnHighlightActor(LastActor);
		HighlightActor(ThisActor);
	}
}

void AGasPlayerController::ShowDamageNumber_Implementation(float DamageAmount, AGasCharacterBase* TargetCharacter,
	FGameplayTagContainer DamageNumberTags)
{
	if (IsValid(TargetCharacter))
	{
		TargetCharacter->AddDamageNumber(DamageAmount, DamageNumberTags);
	}
}

bool AGasPlayerController::ShowDamageNumber_Validate(float DamageAmount, AGasCharacterBase* TargetCharacter,
	FGameplayTagContainer DamageNumberTags)
{
	return true;
}

