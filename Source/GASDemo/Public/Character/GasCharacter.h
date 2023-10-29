// Copyright TanZQ

#pragma once

#include "GasCharacterBase.h"
#include "GasCharacter.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UAlsCameraComponent;
class UInputAction;

UCLASS()
class GASDEMO_API AGasCharacter : public AGasCharacterBase
{
	GENERATED_BODY()

public:
	AGasCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#pragma region Input  需移动到Controller

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
};
