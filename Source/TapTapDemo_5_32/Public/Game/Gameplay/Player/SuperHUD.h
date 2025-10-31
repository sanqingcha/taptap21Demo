// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/Gameplay/Interface/HUDInterface.h"
#include "Game/MVVM/UMG/ViewModeUserWidget.h"
#include "GameFramework/HUD.h"
#include "SuperHUD.generated.h"

class UInputMappingContext;
struct FInputActionValue;
class USettingUserWidget;
class UInputAction;
class UViewModeData;
class UUMGViewModelBase;
class UViewModeUserWidget;
class UPlayerBaseData;

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildSpaceActionPressed,bool,isInBuildSpace);

UCLASS(Blueprintable)
class TAPTAPDEMO_5_32_API ASuperHUD : public AHUD , public IHUDInterface
{
	GENERATED_BODY()
public:
	ASuperHUD();
	/**初始化HUD，被PlayerController调用，代替BeginPlay*/
	virtual void InitialHUD(UPlayerBaseData* BaseData) override;
	/**蓝图重载初始化，被InitialHUD调用*/
	UFUNCTION(BlueprintImplementableEvent)
	void InitialHUD_BP();
	UFUNCTION(BlueprintNativeEvent)
	void OnGameStart();
	UFUNCTION(BlueprintCallable)
	UViewModeUserWidget* CreateViewModeUUserWidget(TSubclassOf<UViewModeUserWidget> WidgetClass,int32 ZOrder);
	template<typename TWidget>
	TWidget* CreateDefaultUUserWidget(APlayerController* PC,TSubclassOf<TWidget> WidgetClass,int32 ZOrder = 0 )
	{
		TWidget* ViewModeWidget = CreateWidget<TWidget>(PC,WidgetClass);
		ViewModeWidget->AddToViewport(ZOrder);
		return ViewModeWidget;
	};

	virtual void RegisterBuildSpaceAction_Interface() override{RegisterSwitchBuildSpaceAction();};
	void PressedESC(const FInputActionValue& Value);
	void PressedBuildSpace(const FInputActionValue& Value);
private:
	void RegisterSwitchBuildSpaceAction();
	
	uint8 isShowCursor		:1 = true;
	uint8 isInBuildSpace	:1 = false;
	uint8 haveBind :1 = false;
protected:
	/**储存玩家基本信息*/
	TObjectPtr<UPlayerBaseData> PlayerData;
	UPROPERTY(EditAnywhere,Category="UI_Input",BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> HUDMapping;
	UPROPERTY(EditAnywhere,Category="UI_Input",BlueprintReadOnly)
	TObjectPtr<UInputAction> ESCAction;
	UPROPERTY(EditAnywhere,Category="UI_Input",BlueprintReadOnly)
	TObjectPtr<UInputAction> BuildSpaceAction;
	UPROPERTY(EditAnywhere,Category="UI_Input",BlueprintReadOnly)
	TSubclassOf<USettingUserWidget> SettingWidgetClass;
	
	UPROPERTY()
	TObjectPtr<USettingUserWidget> SettingWidget;
public:
	UPROPERTY(BlueprintAssignable,BlueprintReadWrite)
	FOnBuildSpaceActionPressed OnBuildSpaceActionPressedDelegate;
};
