// Copyright TanZQ


#include "UI/HUD/GasHUD.h"

#include "UI/Widget/GasUserWidget.h"
// #include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
// #include "UI/WidgetController/SpellMenuWidgetController.h"

UOverlayWidgetController* AGasHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

// UAttributeMenuWidgetController* AGasHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
// {
// 	if (AttributeMenuWidgetController == nullptr)
// 	{
// 		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
// 		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
// 		AttributeMenuWidgetController->BindCallbacksToDependencies();
// 	}
// 	return AttributeMenuWidgetController;
// }
//
// USpellMenuWidgetController* AGasHUD::GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams)
// {
// 	if (SpellMenuWidgetController == nullptr)
// 	{
// 		SpellMenuWidgetController = NewObject<USpellMenuWidgetController>(this, SpellMenuWidgetControllerClass);
// 		SpellMenuWidgetController->SetWidgetControllerParams(WCParams);
// 		SpellMenuWidgetController->BindCallbacksToDependencies();
// 	}
// 	return SpellMenuWidgetController;
// }

void AGasHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_GasHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_GasHUD"));
	
	
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UOverlayUserWidget>(Widget);
	
	if (OverlayWidget)
	{
		OverlayWidget->SetWidgetController(WidgetController);
		
		// Todo 之后切换武器的时候将这段代码优化掉。
		OverlayWidget->SetReticle(ReticleWidgetClass);
	}

	WidgetController->BroadcastInitialValues();
	Widget->AddToViewport();
}

