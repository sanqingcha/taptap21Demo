// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Game/Gameplay/Interface/PlayerControllerInterface.h"
#include "GameFramework/PlayerController.h"
#include "SuperPlayerController.generated.h"

enum class EStartNodeType;
class USkillsRuntimeSubsystem;
struct FGameplayTag;
class USkillInputAsset;
class UInputMappingContext;
class IPlayerPawnInterface;
struct FInputActionValue;
class UInputAction;
class UAttributeSet;
class UPlayerBaseData;
class UAbilitySystemComponent;
/**
 * 
 */




UCLASS()
class TAPTAPDEMO_5_32_API ASuperPlayerController : public APlayerController 
{
	GENERATED_BODY()
public:
	ASuperPlayerController();	
protected:
	/**初始化阶段*/
	virtual void BeginPlay() override;

	void intialControl();
	/**StreamManager调用*/
	UFUNCTION()
	void OnMapOpen();
	/**CheckReource*/
	void CheckResource();
	/**初始化HUD*/
	void InitialHUD();
	/**初始化PlayerPawn*/
	void InitialPawn();
	//TODO:: 暂时先使用委托控制Player的pawn的输入映射
	void OnPlayerInputMappingChanged(bool Activate);
	void OnRotSensitivityChanged(float NewValue){RotMut = NewValue;};
	virtual void SetupInputComponent() override;
	
	
	/**InputAction*/
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="input|Settings")
	TObjectPtr<UInputMappingContext> GameInputContext;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="input|Settings")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="input|Settings")
	TObjectPtr<UInputAction> RotateAction;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="input|Settings")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="input|Settings")
	TObjectPtr<USkillInputAsset> SkillActionsData;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="input|Settings")
	FRotator initRotator =FRotator(20,0,0);
private:
	/**ActionValue*/
	void Move_Action(const FInputActionValue& Value);
	void Rotate_Action(const FInputActionValue& Value);
	void Jump_Action(const FInputActionValue& Value);
	void TryUseSkill(EStartNodeType Type);

protected:
	IPlayerPawnInterface* PlayerPawnInterface = nullptr;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySysComp;
public:
	/**玩家信息*/
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPlayerBaseData> PlayerData;
	static UPlayerBaseData* PlayerData_Static;

private:
	float RotMut = 0.5f;
	USkillsRuntimeSubsystem* SkillRuntimeSys;
};
