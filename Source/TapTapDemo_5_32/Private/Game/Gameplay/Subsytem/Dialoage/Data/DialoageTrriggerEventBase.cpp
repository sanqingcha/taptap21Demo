// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Subsytem/Dialoage/Data/DialoageTrriggerEventBase.h"

#include "Game/Gameplay/Interface/HUDInterface.h"
#include "Game/Gameplay/Player/Data/PlayerBaseData.h"
#include "GameFramework/HUD.h"


void UDialoageTrriggerEventBase::OnTrrigger(UPlayerBaseData* PlayerData)
{
	OnTrriggerBP(PlayerData);
}


void UDialoageTrriggerEventBase::EndTrrigger()
{
	MarkAsGarbage();
}

void UDialoageTrriggerEvent_LoadBuildSpace::OnTrrigger(UPlayerBaseData* PlayerData)
{
	IHUDInterface* HUDinterface = Cast<IHUDInterface>(PlayerData->PC->GetHUD());
	if (HUDinterface)
	{
		HUDinterface->RegisterBuildSpaceAction_Interface();
	}
	EndTrrigger();
}


