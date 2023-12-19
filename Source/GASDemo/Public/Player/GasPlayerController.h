// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GasPlayerController.generated.h"

class UMaterialInstance;
struct FGameplayTagContainer;
class AGasCharacterBase;
class UGasAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponClipIconSignature, UMaterialInstance*, InImage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponStatusSignature, const FText&, InText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponAmmoSignature, int32, InNum);

/**
 * 
 */
UCLASS()
class GASDEMO_API AGasPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AGasPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

private:

	UPROPERTY()
	TObjectPtr<UGasAbilitySystemComponent> GasAbilitySystemComponent;

	UGasAbilitySystemComponent* GetASC();

#pragma region Actor 高亮
private:
	UPROPERTY()
	TObjectPtr<AActor> LastActor;
	
	UPROPERTY()
	TObjectPtr<AActor> ThisActor;
	
	FHitResult ReticleHit;
	
	static void HighlightActor(AActor* InActor);
	static void UnHighlightActor(AActor* InActor);
	void ReticleTrace();

#pragma endregion

#pragma region 武器系统 UI 相关 处理逻辑

public:
	UFUNCTION(BlueprintCallable, Category = "Gas|UI")
	void SetEquippedWeaponClipIconFromSprite(UMaterialInstance* InImage);

	UFUNCTION(BlueprintCallable, Category = "Gas|UI")
	void SetEquippedWeaponStatusText(const FText& StatusText);

	UFUNCTION(BlueprintCallable, Category = "Gas|UI")
	void SetEquippedWeaponNameText(const FText& WeaponNameText);

	UFUNCTION(BlueprintCallable, Category = "Gas|UI")
	void SetClipAmmo(int32 ClipAmmo);

	UFUNCTION(BlueprintCallable, Category = "Gas|UI")
	void SetReserveAmmo(int32 ReserveAmmo);

	// UFUNCTION(BlueprintCallable, Category = "GASShooter|UI")
	// void SetHUDReticle(TSubclassOf<UGasHUDReticle> ReticleClass);
	
	UPROPERTY(BlueprintAssignable, Category="Gas|UI")
	FWeaponClipIconSignature OnWeaponClipIconChanged;
	
	UPROPERTY(BlueprintAssignable, Category="Gas|UI")
	FWeaponStatusSignature OnWeaponStatusChanged;
	
	UPROPERTY(BlueprintAssignable, Category="Gas|UI")
	FWeaponStatusSignature OnWeaponNameChanged;
	
	UPROPERTY(BlueprintAssignable, Category="Gas|UI")
	FWeaponAmmoSignature OnClipAmmoChanged;
	
	UPROPERTY(BlueprintAssignable, Category="Gas|UI")
	FWeaponAmmoSignature OnReserveAmmoChanged;
	
#pragma endregion


#pragma region UI

public:
	UFUNCTION(Client, Reliable, WithValidation)
	void ShowDamageNumber(float DamageAmount, AGasCharacterBase* TargetCharacter, FGameplayTagContainer DamageNumberTags);
	
#pragma endregion 
};
