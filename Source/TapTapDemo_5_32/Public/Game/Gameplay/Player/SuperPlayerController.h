// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerController.h"
#include "SuperPlayerController.generated.h"

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
	/**CheckReource*/
	void CheckResource();
	/**初始化HUD*/
	void InitialHUD();
	/**初始化PlayerPawn*/
	void InitialPawn();
	
	virtual void SetupInputComponent() override;

	/**InputAction*/
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> GameInputContext;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UInputAction> RotateAction;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UInputAction> ShootAction;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<USkillInputAsset> SkillActionsData;
private:
	/**ActionValue*/
	void Move_Action(const FInputActionValue& Value);
	void Rotate_Action(const FInputActionValue& Value);
	void Jump_Action(const FInputActionValue& Value);
	void Shoot_Action(const FInputActionValue& Value);
	void TryUseSkill(FGameplayTag Tag);

protected:
	IPlayerPawnInterface* PlayerPawnInterface = nullptr;
	/**玩家信息*/
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPlayerBaseData> PlayerData;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySysComp;
};
