// Copyright Amos Tan


#include "Character/GasCharacter.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/GasAbilitySystemComponent.h"
#include "AbilitySystem/GasGameplayAbility.h"
#include "AbilitySystem/AttributeSet/GasAmmoAttributeSet.h"
#include "Actors/GasWeapon.h"
#include "ALSCamera/Public/AlsCameraComponent.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Input/GasInputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/GasPlayerController.h"
#include "Player/GasPlayerState.h"
#include "Sound/SoundCue.h"
#include "UI/HUD/GasHUD.h"

AGasCharacter::AGasCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	Camera = CreateDefaultSubobject<UAlsCameraComponent>(FName{TEXTVIEW("Camera")});
	Camera->SetupAttachment(GetMesh());
	Camera->SetRelativeRotation_Direct({0.0f, 90.0f, 0.0f});
}

void AGasCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AGasCharacter, Inventory);
	
	// Only replicate CurrentWeapon to simulated clients and manually sync CurrentWeeapon with Owner when we're ready.
	// This allows us to predict weapon changing.
	DOREPLIFETIME_CONDITION(AGasCharacter, CurrentWeapon, COND_SimulatedOnly);
}

void AGasCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 服务器初始化 ability actor info
	InitAbilityActorInfo();
	InitializeDefaultAttributes();

	AddCharacterAbilities();
}

void AGasCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 客户端 初始化 ability actor info
	InitAbilityActorInfo();

	BindASCInput();
}

void AGasCharacter::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !IsValid(GasAbilitySystemComponent) || !GasAbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}
	
	// 将之前添加的GA都进行删除。
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;

	// TODO 如果父类添加了其他的 GA 需要复制一份到此处。
	//  ... ...
	
	for (const FGameplayAbilitySpec& Spec : GasAbilitySystemComponent->GetActivatableAbilities())
	{
		if ((Spec.SourceObject == this) && ASCInputMappingContext->Abilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}
	
	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	GasAbilitySystemComponent->bCharacterAbilitiesGiven = false;
}

void AGasCharacter::InitAbilityActorInfo()
{
	const auto GasPlayerState = GetPlayerState<AGasPlayerState>();
	check(GasPlayerState);
	
	AbilitySystemComponent = GasPlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(GasPlayerState, this);
	GasAbilitySystemComponent = CastChecked<UGasAbilitySystemComponent>(GasPlayerState->GetAbilitySystemComponent());
	GasAbilitySystemComponent->AbilityActorInfoSet();
	
	AttributeSet = GasPlayerState->GetAttributeSet();
	if (AGasPlayerController* GasPlayerController = Cast<AGasPlayerController>(GetController()))
	{
		if (AGasHUD* GasHUD = Cast<AGasHUD>(GasPlayerController->GetHUD()))
		{
			GasHUD->InitOverlay(GasPlayerController, GasPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}

void AGasCharacter::AddCharacterAbilities()
{
	// 只在服务器上授予GA
	if (GetLocalRole() != ROLE_Authority || !IsValid(GasAbilitySystemComponent) || GasAbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}

	// TODO 如果父类添加了其他的 GA 需要复制一份到此处。
	//  ... ... 

	check(ASCInputMappingContext);
	for (TSubclassOf<UGasGameplayAbility>& StartupAbility : ASCInputMappingContext->Abilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility,
		                                   GetAbilityLevel( StartupAbility.GetDefaultObject()->AbilityID),
		                            static_cast<int32>(StartupAbility.GetDefaultObject()-> AbilityInputID),
		                            this));
	}

	GasAbilitySystemComponent->bCharacterAbilitiesGiven = true;
}

void AGasCharacter::NotifyControllerChanged()
{
	const auto* PreviousPlayer{Cast<APlayerController>(PreviousController)};
	if (IsValid(PreviousPlayer))
	{
		auto* InputSubsystem{ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PreviousPlayer->GetLocalPlayer())};
		if (IsValid(InputSubsystem))
		{
			InputSubsystem->RemoveMappingContext(InputMappingContext);
		}
	}

	auto* NewPlayer{Cast<APlayerController>(GetController())};
	if (IsValid(NewPlayer))
	{
		NewPlayer->InputYawScale_DEPRECATED = 1.0f;
		NewPlayer->InputPitchScale_DEPRECATED = 1.0f;
		NewPlayer->InputRollScale_DEPRECATED = 1.0f;

		auto* InputSubsystem{ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(NewPlayer->GetLocalPlayer())};
		if (IsValid(InputSubsystem))
		{
			FModifyContextOptions Options;
			Options.bNotifyUserSettings = true;

			InputSubsystem->AddMappingContext(InputMappingContext, 0, Options);
		}
	}

	Super::NotifyControllerChanged();
}

void AGasCharacter::CalcCamera(const float DeltaTime, FMinimalViewInfo& ViewInfo)
{
	if (Camera->IsActive())
	{
		Camera->GetViewInfo(ViewInfo);
		return;
	}

	Super::CalcCamera(DeltaTime, ViewInfo);
}

void AGasCharacter::SetupPlayerInputComponent(UInputComponent* Input)
{
	Super::SetupPlayerInputComponent(Input);

	auto* EnhancedInput{Cast<UEnhancedInputComponent>(Input)};
	if (IsValid(EnhancedInput))
	{
		EnhancedInput->BindAction(LookMouseAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnLookMouse);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnLook);
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnMove);
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnSprint);
		EnhancedInput->BindAction(WalkAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnWalk);
		EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnCrouch);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnJump);
		EnhancedInput->BindAction(AimAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnAim);
		EnhancedInput->BindAction(RagdollAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnRagdoll);
		EnhancedInput->BindAction(RollAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnRoll);
		EnhancedInput->BindAction(RotationModeAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnRotationMode);
		EnhancedInput->BindAction(ViewModeAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnViewMode);
		EnhancedInput->BindAction(SwitchShoulderAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnSwitchShoulder);
	}

	BindASCInput();
}

void AGasCharacter::Input_OnLookMouse(const FInputActionValue& ActionValue)
{
	const auto Value{ActionValue.Get<FVector2D>()};

	AddControllerPitchInput(Value.Y * LookUpMouseSensitivity);
	AddControllerYawInput(Value.X * LookRightMouseSensitivity);
}

void AGasCharacter::Input_OnLook(const FInputActionValue& ActionValue)
{
	const auto Value{ActionValue.Get<FVector2D>()};

	AddControllerPitchInput(Value.Y * LookUpRate);
	AddControllerYawInput(Value.X * LookRightRate);
}

void AGasCharacter::Input_OnMove(const FInputActionValue& ActionValue)
{
	const auto Value{UAlsMath::ClampMagnitude012D(ActionValue.Get<FVector2D>())};

	const auto ForwardDirection{UAlsMath::AngleToDirectionXY(UE_REAL_TO_FLOAT(GetViewState().Rotation.Yaw))};
	const auto RightDirection{UAlsMath::PerpendicularCounterClockwiseXY(ForwardDirection)};

	AddMovementInput(ForwardDirection * Value.Y + RightDirection * Value.X);
}

void AGasCharacter::Input_OnSprint(const FInputActionValue& ActionValue)
{
	SetDesiredGait(ActionValue.Get<bool>() ? AlsGaitTags::Sprinting : AlsGaitTags::Running);
}

void AGasCharacter::Input_OnWalk()
{
	if (GetDesiredGait() == AlsGaitTags::Walking)
	{
		SetDesiredGait(AlsGaitTags::Running);
	}
	else if (GetDesiredGait() == AlsGaitTags::Running)
	{
		SetDesiredGait(AlsGaitTags::Walking);
	}
}

void AGasCharacter::Input_OnCrouch()
{
	if (GetDesiredStance() == AlsStanceTags::Standing)
	{
		SetDesiredStance(AlsStanceTags::Crouching);
	}
	else if (GetDesiredStance() == AlsStanceTags::Crouching)
	{
		SetDesiredStance(AlsStanceTags::Standing);
	}
}

void AGasCharacter::Input_OnJump(const FInputActionValue& ActionValue)
{
	if (ActionValue.Get<bool>())
	{
		if (StopRagdolling())
		{
			return;
		}

		if (StartMantlingGrounded())
		{
			return;
		}

		if (GetStance() == AlsStanceTags::Crouching)
		{
			SetDesiredStance(AlsStanceTags::Standing);
			return;
		}

		Jump();
	}
	else
	{
		StopJumping();
	}
}

void AGasCharacter::Input_OnAim(const FInputActionValue& ActionValue)
{
	SetDesiredAiming(ActionValue.Get<bool>());
}

void AGasCharacter::Input_OnRagdoll()
{
	if (!StopRagdolling())
	{
		StartRagdolling();
	}
}

void AGasCharacter::Input_OnRoll()
{
	static constexpr auto PlayRate{1.3f};

	StartRolling(PlayRate);
}

void AGasCharacter::Input_OnRotationMode()
{
	SetDesiredRotationMode(GetDesiredRotationMode() == AlsRotationModeTags::VelocityDirection
		                       ? AlsRotationModeTags::ViewDirection
		                       : AlsRotationModeTags::VelocityDirection);
}

void AGasCharacter::Input_OnViewMode()
{
	SetViewMode(GetViewMode() == AlsViewModeTags::ThirdPerson ? AlsViewModeTags::FirstPerson : AlsViewModeTags::ThirdPerson);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AGasCharacter::Input_OnSwitchShoulder()
{
	Camera->SetRightShoulder(!Camera->IsRightShoulder());
}

int32 AGasCharacter::GetAbilityLevel(EGasAbilityInputID AbilityID) const
{
	return 1;
}

void AGasCharacter::BindASCInput()
{
    if (!ASCInputMappingContext)
    {
        return;
    }
    
	if (!bASCInputBound && IsValid(AbilitySystemComponent) && IsValid(InputComponent))
	{
		GasAbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, ASCInputMappingContext->Mappings);
		
		bASCInputBound = true;
	}
}

void AGasCharacter::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& Unused, float& VerticalLocation)
{
	if (Camera->IsActive())
	{
		Camera->DisplayDebug(Canvas, DisplayInfo, VerticalLocation);
	}

	Super::DisplayDebug(Canvas, DisplayInfo, Unused, VerticalLocation);
}

bool AGasCharacter::AddWeaponToInventory(AGasWeapon* NewWeapon, bool bEquipWeapon)
{
	if (DoesWeaponExistInInventory(NewWeapon))
	{
		USoundCue* PickupSound = NewWeapon->GetPickupSound();

		if (PickupSound && IsLocallyControlled())
		{
			UGameplayStatics::SpawnSoundAttached(PickupSound, GetRootComponent());
		}

		if (GetLocalRole() < ROLE_Authority)
		{
			return false;
		}

		// Create a dynamic instant Gameplay Effect to give the primary and secondary ammo
		UGameplayEffect* GEAmmo = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("Ammo")));
		GEAmmo->DurationPolicy = EGameplayEffectDurationType::Instant;

		if (NewWeapon->PrimaryAmmoType != WeaponAmmoTypeNoneTag)
		{
			int32 Idx = GEAmmo->Modifiers.Num();
			GEAmmo->Modifiers.SetNum(Idx + 1);
		
			FGameplayModifierInfo& InfoPrimaryAmmo = GEAmmo->Modifiers[Idx];
			InfoPrimaryAmmo.ModifierMagnitude = FScalableFloat(NewWeapon->GetPrimaryClipAmmo());
			InfoPrimaryAmmo.ModifierOp = EGameplayModOp::Additive;
			InfoPrimaryAmmo.Attribute = UGasAmmoAttributeSet::GetReserveAmmoAttributeFromTag(NewWeapon->PrimaryAmmoType);
		}
		
		if (NewWeapon->SecondaryAmmoType != WeaponAmmoTypeNoneTag)
		{
			int32 Idx = GEAmmo->Modifiers.Num();
			GEAmmo->Modifiers.SetNum(Idx + 1);
		
			FGameplayModifierInfo& InfoSecondaryAmmo = GEAmmo->Modifiers[Idx];
			InfoSecondaryAmmo.ModifierMagnitude = FScalableFloat(NewWeapon->GetSecondaryClipAmmo());
			InfoSecondaryAmmo.ModifierOp = EGameplayModOp::Additive;
			InfoSecondaryAmmo.Attribute = UGasAmmoAttributeSet::GetReserveAmmoAttributeFromTag(NewWeapon->SecondaryAmmoType);
		}
		
		if (GEAmmo->Modifiers.Num() > 0)
		{
			AbilitySystemComponent->ApplyGameplayEffectToSelf(GEAmmo, 1.0f, AbilitySystemComponent->MakeEffectContext());
		}
		
		NewWeapon->Destroy();

		return false;
	}

	if (GetLocalRole() < ROLE_Authority)
	{
		return false;
	}

	Inventory.Weapons.Add(NewWeapon);
	NewWeapon->SetOwningCharacter(this);
	NewWeapon->AddAbilities();
	
	if (bEquipWeapon)
	{
		EquipWeapon(NewWeapon);
		ClientSyncCurrentWeapon(CurrentWeapon);
	}

	return true;
}

void AGasCharacter::EquipWeapon(AGasWeapon* NewWeapon)
{
	// if (GetLocalRole() < ROLE_Authority)
	// {
	// 	ServerEquipWeapon(NewWeapon);
	// 	SetCurrentWeapon(NewWeapon, CurrentWeapon);
	// 	bChangedWeaponLocally = true;
	// }
	// else
	{
		SetCurrentWeapon(NewWeapon, CurrentWeapon);
	}
}

FName AGasCharacter::GetWeaponAttachPoint() const
{
	FGameplayTag WeaponTag = FGameplayTag::RequestGameplayTag(FName("Als.OverlayMode.NonPistol"));
	if (CurrentWeaponTag.MatchesTag(WeaponTag))
	{
		return NonPistolWeaponAttachPoint;
	}
	
	WeaponTag = FGameplayTag::RequestGameplayTag(FName("Als.OverlayMode.PistolOneHanded"));
	if (CurrentWeaponTag.MatchesTag(WeaponTag))
	{
		return PistolWeaponAttachPoint;
	}
	
	WeaponTag = FGameplayTag::RequestGameplayTag(FName("Als.OverlayMode.PistolTwoHanded"));
	if (CurrentWeaponTag.MatchesTag(WeaponTag))
	{
		return PistolWeaponAttachPoint;
	}
	return NonPistolWeaponAttachPoint;
}

AGasWeapon* AGasCharacter::GetCurrentWeapon() const
{
	return CurrentWeapon;
}

bool AGasCharacter::DoesWeaponExistInInventory(const AGasWeapon* InWeapon)
{
	for (const AGasWeapon* Weapon : Inventory.Weapons)
	{
		if (Weapon && InWeapon && Weapon->GetClass() == InWeapon->GetClass())
		{
			return true;
		}
	}

	return false;
}

void AGasCharacter::SetCurrentWeapon(AGasWeapon* NewWeapon, AGasWeapon* LastWeapon)
{
	if (NewWeapon == LastWeapon)
	{
		return;
	}

	// Cancel active weapon abilities
	if (AbilitySystemComponent)
	{
		FGameplayTagContainer AbilityTagsToCancel = FGameplayTagContainer(WeaponAbilityTag);
		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel);
	}

	UnEquipWeapon(LastWeapon);

	if (NewWeapon)
	{
		if (AbilitySystemComponent)
		{
			// Clear out potential NoWeaponTag
			AbilitySystemComponent->RemoveLooseGameplayTag(CurrentWeaponTag);
		}

		// Weapons coming from OnRep_CurrentWeapon won't have the owner set
		CurrentWeapon = NewWeapon;
		CurrentWeapon->SetOwningCharacter(this);
		CurrentWeapon->Equip();
		CurrentWeaponTag = CurrentWeapon->WeaponTag;

		SetOverlayMode(CurrentWeaponTag);

		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->AddLooseGameplayTag(CurrentWeaponTag);
		}

		// TODO UI 相关的逻辑 
		// AGasPlayerController* PC = GetController<AGasPlayerController>();
		// if (PC && PC->IsLocalController())
		// {
		// 	PC->SetEquippedWeaponPrimaryIconFromSprite(CurrentWeapon->PrimaryIcon);
		// 	PC->SetEquippedWeaponStatusText(CurrentWeapon->StatusText);
		// 	PC->SetPrimaryClipAmmo(CurrentWeapon->GetPrimaryClipAmmo());
		// 	PC->SetPrimaryReserveAmmo(GetPrimaryReserveAmmo());
		// 	PC->SetHUDReticle(CurrentWeapon->GetPrimaryHUDReticleClass());
		// }

		// TODO 配置弹药更新时触发的逻辑
		// NewWeapon->OnPrimaryClipAmmoChanged.AddDynamic(this, &AGasCharacter::CurrentWeaponPrimaryClipAmmoChanged);
		// NewWeapon->OnSecondaryClipAmmoChanged.AddDynamic(this, &AGasCharacter::CurrentWeaponSecondaryClipAmmoChanged);
		
		// if (AbilitySystemComponent)
		// {
		// 	PrimaryReserveAmmoChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGasAmmoAttributeSet::GetReserveAmmoAttributeFromTag(CurrentWeapon->PrimaryAmmoType)).AddUObject(this, &AGSHeroCharacter::CurrentWeaponPrimaryReserveAmmoChanged);
		// 	SecondaryReserveAmmoChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGasAmmoAttributeSet::GetReserveAmmoAttributeFromTag(CurrentWeapon->SecondaryAmmoType)).AddUObject(this, &AGSHeroCharacter::CurrentWeaponSecondaryReserveAmmoChanged);
		// }

		// TODO 播放切换武器蒙太奇
		// UAnimMontage* Equip1PMontage = CurrentWeapon->GetEquip1PMontage();
		// if (Equip1PMontage && GetFirstPersonMesh())
		// {
		// 	GetFirstPersonMesh()->GetAnimInstance()->Montage_Play(Equip1PMontage);
		// }
		//
		// UAnimMontage* Equip3PMontage = CurrentWeapon->GetEquip3PMontage();
		// if (Equip3PMontage && GetThirdPersonMesh())
		// {
		// 	GetThirdPersonMesh()->GetAnimInstance()->Montage_Play(Equip3PMontage);
		// }
	}
	else
	{
		// This will clear HUD, tags etc
		UnEquipCurrentWeapon();
	}
}

void AGasCharacter::UnEquipWeapon(AGasWeapon* WeaponToUnEquip)
{
	//TODO this will run into issues when calling UnEquipWeapon explicitly and the WeaponToUnEquip == CurrentWeapon

	if (WeaponToUnEquip)
	{
		// WeaponToUnEquip->OnPrimaryClipAmmoChanged.RemoveDynamic(this, &AGasCharacter::CurrentWeaponPrimaryClipAmmoChanged);
		// WeaponToUnEquip->OnSecondaryClipAmmoChanged.RemoveDynamic(this, &AGasCharacter::CurrentWeaponSecondaryClipAmmoChanged);
		//
		// if (AbilitySystemComponent)
		// {
		// 	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGSAmmoAttributeSet::GetReserveAmmoAttributeFromTag(WeaponToUnEquip->PrimaryAmmoType)).Remove(PrimaryReserveAmmoChangedDelegateHandle);
		// 	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGSAmmoAttributeSet::GetReserveAmmoAttributeFromTag(WeaponToUnEquip->SecondaryAmmoType)).Remove(SecondaryReserveAmmoChangedDelegateHandle);
		// }
		
		WeaponToUnEquip->UnEquip();
	}
}

void AGasCharacter::UnEquipCurrentWeapon()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(CurrentWeaponTag);
		CurrentWeaponTag = NoWeaponTag;
		AbilitySystemComponent->AddLooseGameplayTag(CurrentWeaponTag);
	}

	UnEquipWeapon(CurrentWeapon);
	CurrentWeapon = nullptr;

	// TODO  UI 
	// AGSPlayerController* PC = GetController<AGSPlayerController>();
	// if (PC && PC->IsLocalController())
	// {
	// 	PC->SetEquippedWeaponPrimaryIconFromSprite(nullptr);
	// 	PC->SetEquippedWeaponStatusText(FText());
	// 	PC->SetPrimaryClipAmmo(0);
	// 	PC->SetPrimaryReserveAmmo(0);
	// 	PC->SetHUDReticle(nullptr);
	// }
}

void AGasCharacter::ClientSyncCurrentWeapon_Implementation(AGasWeapon* InWeapon)
{
	AGasWeapon* LastWeapon = CurrentWeapon;
	CurrentWeapon = InWeapon;
	OnRep_CurrentWeapon(LastWeapon);
}

bool AGasCharacter::ClientSyncCurrentWeapon_Validate(AGasWeapon* InWeapon)
{
	return true;
}

void AGasCharacter::OnRep_Inventory()
{
	// TODO 后续再处理多端同步问题。
}

void AGasCharacter::OnRep_CurrentWeapon(AGasWeapon* LastWeapon)
{
	bChangedWeaponLocally = false;
	SetCurrentWeapon(CurrentWeapon, LastWeapon);
}
