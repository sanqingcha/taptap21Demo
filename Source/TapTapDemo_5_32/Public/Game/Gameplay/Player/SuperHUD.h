// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/Gameplay/Interface/HUDInterface.h"
#include "Game/MVVM/UMG/ViewModeUserWidget.h"
#include "GameFramework/HUD.h"
#include "SuperHUD.generated.h"

class UViewModeData;
class UUMGViewModelBase;
class UViewModeUserWidget;
class UPlayerBaseData;
/**
 * 
 */
UCLASS()
class TAPTAPDEMO_5_32_API ASuperHUD : public AHUD , public IHUDInterface
{
	GENERATED_BODY()
public:
	/**初始化HUD，被PlayerController调用，代替BeginPlay*/
	virtual void InitialHUD(UPlayerBaseData* BaseData) override;
	/**蓝图重载初始化，被InitialHUD调用*/
	UFUNCTION(BlueprintImplementableEvent)
	void InitialHUD_BP();
	UFUNCTION(BlueprintCallable)
	UViewModeUserWidget* CreateViewModeUUserWidget(TSubclassOf<UViewModeUserWidget> WidgetClass);
	/**储存玩家基本信息*/
	TObjectPtr<UPlayerBaseData> PlayerData;
	
};
