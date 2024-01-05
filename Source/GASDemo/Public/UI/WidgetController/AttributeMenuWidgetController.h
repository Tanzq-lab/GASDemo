// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"
#include "GasWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FGameplayAttribute;
struct FGameplayTag;
class UGasAttributeSet;
class UGasAbilitySystemComponent;
class AGasPlayerState;
class AGasPlayerController;
class UAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FGasAttributeInfo&, Info);

/**
 * 
 */
UCLASS()
class GASDEMO_API UAttributeMenuWidgetController : public UGasWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable, Category="Gas|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

	AGasPlayerController* GetGasPlayerController();
	AGasPlayerState* GetGasPlayerState();
	UGasAbilitySystemComponent* GetGasAbilitySystemComponent();
	UGasAttributeSet* GetGasAttributeSet();
	
protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;
	
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<AGasPlayerController> GasPlayerController;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<AGasPlayerState> GasPlayerState;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UGasAbilitySystemComponent> GasAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UGasAttributeSet> GasAttributeSet;
	
private:

	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
