// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GasWeapon.generated.h"


enum class EGasAbilityInputID : uint8;
struct FGameplayAbilitySpecHandle;
class UGasGameplayAbility;
class UGasAbilitySystemComponent;
class AGasCharacter;
class USoundCue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponAmmoChangedDelegate, int32, OldValue, int32, NewValue);

UCLASS(Blueprintable, BlueprintType)
class GASDEMO_API AGasWeapon : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

#pragma region 初始化相关逻辑
	
public:
	AGasWeapon();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma endregion 

	
#pragma region 交互逻辑
	
public:
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void SetOwningCharacter(AGasCharacter* InOwningCharacter);

	virtual void AddAbilities();

	virtual int32 GetAbilityLevel(EGasAbilityInputID AbilityID);

	// Called when the player equips this weapon
	virtual void Equip();

	// Called when the player unequips this weapon
	virtual void UnEquip();

protected:
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Gas|GSWeapon")
	AGasCharacter* OwningCharacter;

	UPROPERTY()
	UGasAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, Category = "Gas|GSWeapon")
	TArray<TSubclassOf<UGasGameplayAbility>> Abilities;

	UPROPERTY(BlueprintReadOnly, Category = "Gas|GSWeapon")
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
	
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	
	/**
	 * 当有角色拾取该武器时调用
	 */
	virtual void PickUpOnTouch(AGasCharacter* InCharacter);

#pragma endregion 

#pragma region 数据配置

public:
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Gas|GSWeapon")
	FGameplayTag PrimaryAmmoType;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Gas|GSWeapon")
	FGameplayTag SecondaryAmmoType;

	// This tag is primarily used by the first person Animation Blueprint to determine which animations to play
	// (Rifle vs Rocket Launcher)
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Gas|GSWeapon")
	FGameplayTag WeaponTag;
	
	UFUNCTION(BlueprintCallable, Category = "Gas|Audio")
	USoundCue* GetPickupSound() const;

	UFUNCTION(BlueprintCallable, Category = "Gas|GSWeapon")
	virtual int32 GetPrimaryClipAmmo() const;

	UFUNCTION(BlueprintCallable, Category = "Gas|GSWeapon")
	virtual int32 GetMaxPrimaryClipAmmo() const;

	UFUNCTION(BlueprintCallable, Category = "Gas|GSWeapon")
	virtual int32 GetSecondaryClipAmmo() const;

	UFUNCTION(BlueprintCallable, Category = "Gas|GSWeapon")
	virtual int32 GetMaxSecondaryClipAmmo() const;
protected:
	/**
	 * 当武器被拾取时播放的音效
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Gas|Audio")
	USoundCue* PickupSound;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, ReplicatedUsing = OnRep_PrimaryClipAmmo, Category = "Gas|GSWeapon|Ammo")
	int32 PrimaryClipAmmo;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, ReplicatedUsing = OnRep_MaxPrimaryClipAmmo, Category = "Gas|GSWeapon|Ammo")
	int32 MaxPrimaryClipAmmo;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, ReplicatedUsing = OnRep_SecondaryClipAmmo, Category = "Gas|GSWeapon|Ammo")
	int32 SecondaryClipAmmo;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, ReplicatedUsing = OnRep_MaxSecondaryClipAmmo, Category = "Gas|GSWeapon|Ammo")
	int32 MaxSecondaryClipAmmo;


	UFUNCTION()
	virtual void OnRep_PrimaryClipAmmo(int32 OldPrimaryClipAmmo);

	UFUNCTION()
	virtual void OnRep_MaxPrimaryClipAmmo(int32 OldMaxPrimaryClipAmmo);

	UFUNCTION()
	virtual void OnRep_SecondaryClipAmmo(int32 OldSecondaryClipAmmo);

	UFUNCTION()
	virtual void OnRep_MaxSecondaryClipAmmo(int32 OldMaxSecondaryClipAmmo);
#pragma endregion

#pragma region 被装备后的逻辑

public:

	// TODO 后续设置这些数据的时候 将这些数据进行运用。
	
	UPROPERTY(BlueprintAssignable, Category = "Gas|GSWeapon")
	FWeaponAmmoChangedDelegate OnPrimaryClipAmmoChanged;

	UPROPERTY(BlueprintAssignable, Category = "Gas|GSWeapon")
	FWeaponAmmoChangedDelegate OnMaxPrimaryClipAmmoChanged;

	UPROPERTY(BlueprintAssignable, Category = "Gas|GSWeapon")
	FWeaponAmmoChangedDelegate OnSecondaryClipAmmoChanged;

	UPROPERTY(BlueprintAssignable, Category = "Gas|GSWeapon")
	FWeaponAmmoChangedDelegate OnMaxSecondaryClipAmmoChanged;
	
#pragma endregion 
};
