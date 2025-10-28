// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/Gameplay/Interface/InputMappingInterface.h"
#include "GameFramework/Actor.h"
#include "SingleCameraActor.generated.h"

class ISkillNodeInteractInterface;
class UCameraRegisterComponent;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActivateInput,bool , Activate);

UCLASS()
class TAPTAPDEMO_5_32_API ASingleCameraActor : public AActor , public IInputMappingInterface
{
	GENERATED_BODY()
public:	
	ASingleCameraActor();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="CheckTrace|CameraInput")
	TArray<TEnumAsByte<EObjectTypeQuery> > CheckObjectTypes;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UCameraRegisterComponent> CameraRegisterCompoent;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<USceneComponent> RootSceneComp;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UCameraComponent> CameraComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Action|CameraInput")
	TObjectPtr<UInputMappingContext> Mapping;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Action|CameraInput",DisplayName="MoveAction")
	TObjectPtr<UInputAction> IA_Move;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Action|CameraInput",DisplayName="ZoomAction")
	TObjectPtr<UInputAction> IA_Zoom;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Action|CameraInput",DisplayName="HouldAction")
	TObjectPtr<UInputAction> IA_Hold;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Action|CameraInput",DisplayName="HouldAction")
	TObjectPtr<UInputAction> IA_Shift_Modifier;
	/**相对于Actor开始时候的位置的最大距离*/
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Move|CameraInput",DisplayName = "MoveRange")
	float MoveRangeQuare = 300.f;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Move|CameraInput")
	float Speed = 100.f;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Move|CameraInput")
	float MaxSpringLength = 300;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Move|CameraInput")
	float MinSpringLenhth = 600;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Move|CameraInput")
	float ZoomSpeed = 0.1f;
	
	
	float SpringLengthTarget;
	
	void Move_Action(const FInputActionValue& Value);
	void Zoom_Action(const FInputActionValue& Value);
	void Pressed_Action(const FInputActionValue& Value);
	void Release_Action(const FInputActionValue& Value);

	void ToHoldingNodeActor();
	void ToReleaseNodeAction();
	
	void UpdateInputControl(float DeltaTime);
	void MouseFloatingAction(float DeltaTime);
	void UpdateSpringArmLength(float DeltaTime);
	
	virtual void ActivateCamera(UPlayerBaseData* inPlayerData) override;
	virtual void DeactivateCamera(UPlayerBaseData* inPlayerData) override;
	UFUNCTION()
	void BindMapping(bool isActivate);
	void ShiftModifier(const FInputActionValue& Value, bool Pressed) {ShiftPressed=Pressed;};
	FOnActivateInput OnActivateInputDelegate;
private:
	bool isActive = false;
	UPlayerBaseData* PlayerData;
	
	AActor* CurrMouseNode;//当前鼠标下的Actor，为nullptr就是没有；
	ISkillNodeInteractInterface* FollowNodeInterface;
	uint8 isHolding : 1 = false;
	uint8 ReleasedIsVaild : 1 = false;
	uint8 PressedIsVaild : 1 = false;
	uint8 ShiftPressed :1 = false;
	FTimerHandle HoldTimerHandle;
	FVector CurrNodePos = FVector(0, 0, 0);/**节点位置快照，用于重叠返回*/
	
	void NodeFollowing();
	void DelayCheck();
	
	ISkillNodeInteractInterface* SelectHead = nullptr;
	ISkillNodeInteractInterface* SelectTail = nullptr;

	float CheckTime = 0.1f;	
};
