// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GasWeapon.generated.h"


class AGasGATA_LineTrace;
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

	virtual void BeginPlay() override;

#pragma endregion 

	
#pragma region 交互逻辑
	
public:
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void SetOwningCharacter(AGasCharacter* InOwningCharacter);

	virtual void AddAbilities();

	virtual int32 GetAbilityLevel(EGasAbilityInputID AbilityID);

	// 当玩家装配武器的时候调用
	virtual void Equip();

	// 当玩家卸载武器的时候调用
	virtual void UnEquip();

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetWeaponMesh() const;

protected:
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Gas|Weapon")
	AGasCharacter* OwningCharacter;

	UPROPERTY(VisibleAnywhere, Category = "GASShooter|Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY()
	UGasAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, Category = "Gas|Weapon")
	TArray<TSubclassOf<UGasGameplayAbility>> Abilities;

	UPROPERTY(BlueprintReadOnly, Category = "Gas|Weapon")
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
	
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	
	/**
	 * 当有角色拾取该武器时调用
	 */
	virtual void PickUpOnTouch(AGasCharacter* InCharacter);

#pragma endregion 

#pragma region 武器属性

public:
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Gas|Weapon")
	FGameplayTag FireMode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gas|Weapon")
	FGameplayTag DefaultFireMode;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Gas|Weapon")
	FGameplayTag AmmoType;

	// 对应着当前的武器类型
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Gas|Weapon")
	FGameplayTag WeaponTag;
	
	UFUNCTION(BlueprintCallable, Category = "Gas|Audio")
	USoundCue* GetPickupSound() const;

	UFUNCTION(BlueprintCallable, Category = "Gas|Weapon")
	virtual int32 GetClipAmmo() const;

	UFUNCTION(BlueprintCallable, Category = "Gas|Weapon")
	virtual int32 GetMaxClipAmmo() const;

	UFUNCTION(BlueprintCallable, Category = "Gas|GSWeapon")
	virtual void SetClipAmmo(int32 NewClipAmmo);
	
protected:
	/**
	 * 当武器被拾取时播放的音效
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Gas|Audio")
	USoundCue* PickupSound;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, ReplicatedUsing = OnRep_ClipAmmo, Category = "Gas|Weapon|Ammo")
	int32 ClipAmmo;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, ReplicatedUsing = OnRep_MaxClipAmmo, Category = "Gas|Weapon|Ammo")
	int32 MaxClipAmmo;

	/**
	 * 该武器是否没有子弹限制
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gas|Weapon|Ammo")
	bool bInfiniteAmmo;
	
	UFUNCTION(BlueprintCallable, Category = "Gas|GSWeapon")
	virtual bool HasInfiniteAmmo() const;
	
	UFUNCTION()
	virtual void OnRep_ClipAmmo(int32 OldClipAmmo);

	UFUNCTION()
	virtual void OnRep_MaxClipAmmo(int32 OldMaxClipAmmo);
	
#pragma endregion

#pragma region 被装备后的逻辑

public:

	// TODO 后续设置这些数据的时候 将这些数据进行运用。
	
	UPROPERTY(BlueprintAssignable, Category = "Gas|Weapon")
	FWeaponAmmoChangedDelegate OnClipAmmoChanged;

	UPROPERTY(BlueprintAssignable, Category = "Gas|Weapon")
	FWeaponAmmoChangedDelegate OnMaxClipAmmoChanged;

	UPROPERTY()
	AGasGATA_LineTrace* LineTraceTargetActor;

	// Getter for LineTraceTargetActor. Spawns it if it doesn't exist yet.
	UFUNCTION(BlueprintCallable, Category = "Gas|Targeting")
	AGasGATA_LineTrace* GetLineTraceTargetActor();

	// Resets things like fire mode to default
	UFUNCTION(BlueprintCallable, Category = "Gas|GSWeapon")
	virtual void ResetWeapon();
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
#pragma endregion 
};
