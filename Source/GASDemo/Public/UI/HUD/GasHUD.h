// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Widget/OverlayUserWidget.h"
#include "GasHUD.generated.h"

class UAttributeMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
class UOverlayWidgetController;
class UGasUserWidget;
struct FWidgetControllerParams;
// class USpellMenuWidgetController;
/**
 * 
 */
UCLASS()
class GASDEMO_API AGasHUD : public AHUD
{
	GENERATED_BODY()

public:

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	// UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);
	// USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

protected:


private:

	UPROPERTY()
	TObjectPtr<UOverlayUserWidget>  OverlayWidget;	

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayUserWidget> OverlayWidgetClass;

	// Todo 之后切换武器的时候将这个变量优化掉。
	UPROPERTY(EditAnywhere)
	TSubclassOf<UReticleWidget> ReticleWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	// UPROPERTY()
	// TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
	//
	// UPROPERTY(EditAnywhere)
	// TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	// UPROPERTY()
	// TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;
	//
	// UPROPERTY(EditAnywhere)
	// TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;
	
};
