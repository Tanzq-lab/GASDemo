// Copyright TanZQ


#include "UI/Widget/GasUserWidget.h"

void UGasUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
