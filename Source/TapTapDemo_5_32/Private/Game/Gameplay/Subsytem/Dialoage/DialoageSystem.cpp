// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Subsytem/Dialoage/DialoageSystem.h"

#include "GameplayTagContainer.h"
#include "Game/Gameplay/Subsytem/Dialoage/Data/DialoageAsset.h"


void UDialoageSystem::LoadAsset(UDialoageAsset* Asset)
{
	DialoageAsset = Asset;
	OnAssetLoadDelegate.Broadcast(Asset);
}

void UDialoageSystem::RegisterDialoageTable(UDialoageAsset* inDialoageAsset)
{
	LoadAsset(inDialoageAsset);
}

void UDialoageSystem::TrriggerDialoage(const FGameplayTag& TrriggerTag)
{
	check(DialoageAsset);
	FDialoageData* FindData = DialoageAsset->DialoageMapData.Find(TrriggerTag);
	if (!FindData)
	{
		
		UE_LOG(LogTemp, Error, TEXT("TriggerDialoaget :: CantFindDialoageStruct"));
		return;
	}
	//BreakDialoage(TrriggerTag);
	OnDialoageTrriggerDelegate.Broadcast(TrriggerTag,FindData);
}

void UDialoageSystem::BreakDialoage(const FGameplayTag& DialoageKey)
{
	/**被废弃，现在是每次调用触发对话直接打断前一句，不在考虑不能打断上一句的情况了*/
	 check(0);
}

void UDialoageSystem::BreakAllDialoage()
{
	 OnDialoageBreakDelegate.Broadcast({},true);
}
