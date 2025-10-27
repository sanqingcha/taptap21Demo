// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Player/SuperHUD.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "Game/Gameplay/DefaultWidget/SettingUserWidget.h"
#include "Game/Gameplay/Player/Data/PlayerBaseData.h"
#include "Game/Gameplay/Subsytem/GISubSystem/MapSteamLoadSystem.h"
#include "Game/GameplayTags/GameTags.h"
#include "Game/MVVM/UMG/ViewModeUserWidget.h"

ASuperHUD::ASuperHUD()
{
	
}

void ASuperHUD::InitialHUD(UPlayerBaseData* BaseData)
{
	PlayerData = BaseData;
	InitialHUD_BP();
	UMapSteamLoadSystem::GetOnStreamLoadOverDelegate().AddDynamic(this,&ASuperHUD::OnGameStart);

	SettingWidget = CreateDefaultUUserWidget<USettingUserWidget>(PlayerData->PC,SettingWidgetClass,100);
	SettingWidget->SetVisibility(ESlateVisibility::Hidden);
	SettingWidget->OnReturnButtonDelegate.BindUObject(this,&ASuperHUD::PressedESC);
	
	check(SettingWidget);
	check(HUDMapping);

}

void ASuperHUD::OnGameStart_Implementation()
{
	UMapSteamLoadSystem::GetOnStreamLoadOverDelegate().RemoveDynamic(this,&ASuperHUD::OnGameStart);
	PlayerData->EnhancedInputSys->AddMappingContext(HUDMapping,0);
	PlayerData->EnhancedInputComp->BindAction(ESCAction,ETriggerEvent::Started,this,&ASuperHUD::PressedESC);
}

UViewModeUserWidget* ASuperHUD::CreateViewModeUUserWidget(TSubclassOf<UViewModeUserWidget> WidgetClass,int32 ZOrder)
{
	UViewModeUserWidget* ViewModeWidget = CreateWidget<UViewModeUserWidget>(PlayerData->PC,WidgetClass);
	ViewModeWidget->Initial(PlayerData);
	ViewModeWidget->AddToViewport(ZOrder);
	return ViewModeWidget;
}

void ASuperHUD::RegisterSwitchBuildSpaceAction()
{
	PlayerData->EnhancedInputComp->BindAction(BuildSpaceAction,ETriggerEvent::Started,this,&ASuperHUD::PressedBuildSpace);
}

void ASuperHUD::PressedESC(const FInputActionValue& Value)
{
	switch (SettingWidget->GetVisibility())
	{
		case ESlateVisibility::Hidden:
			{
				SettingWidget->SetVisibility(ESlateVisibility::Visible);
				isShowCursor = PlayerData->PC->bShowMouseCursor;
				PlayerData->PC->SetShowMouseCursor(true);
 				UAbilitySystemComponent* ASC = PlayerData->ASC;
 				FGameplayTagContainer BlockInputGameplayTags;
 				BlockInputGameplayTags.AddTag(Game::Input::Game_Input_AttackBlock);
 				BlockInputGameplayTags.AddTag(Game::Input::Game_Input_JumpBlock);
 				BlockInputGameplayTags.AddTag(Game::Input::Game_Input_MoveBlock);
 				BlockInputGameplayTags.AddTag(Game::Input::Game_Input_RotateBlock);
 				ASC->AddLooseGameplayTags(BlockInputGameplayTags);
				break;
			}
		default:
			{
				PlayerData->PC->SetShowMouseCursor(isShowCursor);
				UAbilitySystemComponent* ASC = PlayerData->ASC;
				FGameplayTagContainer BlockInputGameplayTags;
				BlockInputGameplayTags.AddTag(Game::Input::Game_Input_AttackBlock);
				BlockInputGameplayTags.AddTag(Game::Input::Game_Input_JumpBlock);
				BlockInputGameplayTags.AddTag(Game::Input::Game_Input_MoveBlock);
				BlockInputGameplayTags.AddTag(Game::Input::Game_Input_RotateBlock);
				ASC->RemoveLooseGameplayTags(BlockInputGameplayTags);
				SettingWidget->SetVisibility(ESlateVisibility::Hidden);
			}
	}
}

void ASuperHUD::PressedBuildSpace(const FInputActionValue& Value)
{
	isInBuildSpace = !isInBuildSpace;
	OnBuildSpaceActionPressedDelegate.Broadcast(isInBuildSpace);
}


