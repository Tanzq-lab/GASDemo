// Copyright Amos Tan

#pragma once

#include "GasCharacterBase.h"
#include "GasCharacter.generated.h"

class AGasWeapon;
struct FInputActionValue;
class UInputMappingContext;
class UAlsCameraComponent;
class UInputAction;

USTRUCT()
struct GASDEMO_API FGasInventory
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<AGasWeapon*> Weapons;

	// Consumable items

	// Passive items like armor

	// Door keys

	// Etc
};

UCLASS()
class GASDEMO_API AGasCharacter : public AGasCharacterBase
{
	GENERATED_BODY()

#pragma region 初始化相关逻辑
public:
	AGasCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

protected:
	virtual void InitAbilityActorInfo() override;

#pragma endregion 

#pragma region Input

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Gas Character", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Gas Character", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> LookMouseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Gas Character", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Gas Character", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Gas Character", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Gas Character", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> WalkAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Gas Character", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Gas Character", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Gas Character", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Gas Character", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> RagdollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Gas Character", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> RollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Gas Character", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> RotationModeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Gas Character", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> ViewModeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Gas Character", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SwitchShoulderAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Gas Character", Meta = (ClampMin = 0, ForceUnits = "x"))
	float LookUpMouseSensitivity{1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Gas Character", Meta = (ClampMin = 0, ForceUnits = "x"))
	float LookRightMouseSensitivity{1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Gas Character",
		Meta = (ClampMin = 0, ForceUnits = "deg/s"))
	float LookUpRate{90.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Gas Character",
		Meta = (ClampMin = 0, ForceUnits = "deg/s"))
	float LookRightRate{240.0f};
	
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* Input) override;

private:
	void Input_OnLookMouse(const FInputActionValue& ActionValue);

	void Input_OnLook(const FInputActionValue& ActionValue);

	void Input_OnMove(const FInputActionValue& ActionValue);

	void Input_OnSprint(const FInputActionValue& ActionValue);

	void Input_OnWalk();

	void Input_OnCrouch();

	void Input_OnJump(const FInputActionValue& ActionValue);

	void Input_OnAim(const FInputActionValue& ActionValue);

	void Input_OnRagdoll();

	void Input_OnRoll();

	void Input_OnRotationMode();

	void Input_OnViewMode();

	void Input_OnSwitchShoulder();

#pragma endregion

#pragma region Camera
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gas Character")
	TObjectPtr<UAlsCameraComponent> Camera;

	virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& ViewInfo) override;
	
public:
	virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& Unused, float& VerticalLocation) override;
#pragma endregion

#pragma region 武器相关逻辑

public:
	FGameplayTag CurrentWeaponTag;
	
	/**
	 * 增加一个新武器到背包中。
	 * @param NewWeapon 新武器
	 * @param bEquipWeapon 是否立刻装备新武器。
	 * @return false : 已经存在一个相同的武器了。  true: 成功添加一个新武器。
	 */
	UFUNCTION(BlueprintCallable, Category = "Gas|Inventory")
	bool AddWeaponToInventory(AGasWeapon* NewWeapon, bool bEquipWeapon = false);

	UFUNCTION(BlueprintCallable, Category = "Gas|Inventory")
	void EquipWeapon(AGasWeapon* NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "Gas|Inventory")
	FName GetWeaponAttachPoint() const;
	
	UFUNCTION(BlueprintCallable, Category = "Gas|Inventory")
	AGasWeapon* GetCurrentWeapon() const;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_Inventory)
	FGasInventory Inventory;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon)
	AGasWeapon* CurrentWeapon;

	// Set to true when we change the weapon predictively and flip it to false when the Server replicates to confirm.
	// We use this if the Server refused a weapon change ability's activation to ask the Server to sync the client back up
	// with the correct CurrentWeapon.
	bool bChangedWeaponLocally;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gas|GasCharacter")
	FName PistolWeaponAttachPoint;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gas|GasCharacter")
	FName NonPistolWeaponAttachPoint;
	
	FGameplayTag WeaponAmmoTypeNoneTag;
	FGameplayTag WeaponAbilityTag;
	FGameplayTag NoWeaponTag;

	UFUNCTION()
	void OnRep_Inventory();

	UFUNCTION()
	void OnRep_CurrentWeapon(AGasWeapon* LastWeapon);
	
	bool DoesWeaponExistInInventory(const AGasWeapon* InWeapon);
	
	// The CurrentWeapon is only automatically replicated to simulated clients.
	// Use this function to manually sync the autonomous client's CurrentWeapon when we're ready to.
	// This allows us to predict weapon changes (changing weapons fast multiple times in a row so that the server doesn't
	// replicate and clobber our CurrentWeapon).
	UFUNCTION(Client, Reliable, WithValidation)
	void ClientSyncCurrentWeapon(AGasWeapon* InWeapon);

	void SetCurrentWeapon(AGasWeapon* NewWeapon, AGasWeapon* LastWeapon);

	// Unequips the specified weapon. Used when OnRep_CurrentWeapon fires.
	void UnEquipWeapon(AGasWeapon* WeaponToUnEquip);

	// Unequips the current weapon. Used if for example we drop the current weapon.
	void UnEquipCurrentWeapon();

#pragma endregion 
	
};
