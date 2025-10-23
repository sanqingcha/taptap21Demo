// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Player/SuperHUD.h"
#include "Game/Gameplay/Player/Data/PlayerBaseData.h"
#include "Game/MVVM/UMG/ViewModeUserWidget.h"

void ASuperHUD::InitialHUD(UPlayerBaseData* BaseData)
{
	PlayerData = BaseData;
	InitialHUD_BP();
}

UViewModeUserWidget* ASuperHUD::CreateViewModeUUserWidget(TSubclassOf<UViewModeUserWidget> WidgetClass)
{
	UViewModeUserWidget* ViewModeWidget = CreateWidget<UViewModeUserWidget>(PlayerData->PC,WidgetClass);
	ViewModeWidget->Initial(PlayerData);
	ViewModeWidget->AddToViewport();
	return ViewModeWidget;
}


