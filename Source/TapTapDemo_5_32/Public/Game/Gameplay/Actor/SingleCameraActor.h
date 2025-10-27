// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/Gameplay/Interface/InputMappingInterface.h"
#include "GameFramework/Actor.h"
#include "SingleCameraActor.generated.h"

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
	void Hold_Action(const FInputActionValue& Value);
	void Holding_Action(const FInputActionValue& Value);
	void Release_Action(const FInputActionValue& Value);
	
	void UpdateInputControl(float DeltaTime);
	void MouseFloatingAction(float DeltaTime);
	void UpdateSpringArmLength(float DeltaTime);
	
	virtual void ActivateCamera(UPlayerBaseData* inPlayerData) override;
	virtual void DeactivateCamera(UPlayerBaseData* inPlayerData) override;
	UFUNCTION()
	void BindMapping(bool isActivate);
	
	FOnActivateInput OnActivateInputDelegate;
private:
	bool isActive = false;
	UPlayerBaseData* PlayerData;
	
	AActor* HoldingNode;

	bool isHolding = false;
};
