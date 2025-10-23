// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Subsytem/GISubSystem/MapSteamLoadSystem.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"

FStreamMapInfo& UMapSteamLoadSystem::PreLoadLevel(const TSoftObjectPtr<UWorld> Level)
{
	/**移除掉尾部的后缀*/
	const FString AssetPath =  Level.ToString();
	int32 lastIndex = 0;
	check(AssetPath.FindChar('.',lastIndex));
	const FString PathSubStr =  UKismetStringLibrary::GetSubstring(AssetPath, 0,lastIndex); 
	/**end*/
	check(PathSubStr.Len() > 0);
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(),Level,false,true,FLatentActionInfo());
	FStreamMapInfo Data(UGameplayStatics::GetStreamingLevel(GetWorld(), *PathSubStr));
	Data.StreamingLevel->SetShouldBeVisible(false);
	return StreamingLevelMap.Emplace(Level,Data);
}

void UMapSteamLoadSystem::OpenMapBySys(const TSoftObjectPtr<UWorld> Level, bool RemovePrevMap, EMapLoadType LoadType)
{
	UWorld* World = GetWorld();
	if (World == nullptr)return;
	for (auto&i:StreamingLevelMap)
	{
		if (i.Value.HaveLoad&&i.Value.CanbeRemove)
		{
			UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(World,i.Key,FLatentActionInfo(),false);	
		}
	}
	auto* StreamMapIfo =  StreamingLevelMap.Find(Level);
	if (StreamMapIfo == nullptr)
	{
		StreamMapIfo = &PreLoadLevel(Level);
	}
	StreamMapIfo->CanbeRemove = (LoadType == EMapLoadType::Default);
	StreamMapIfo->HaveLoad = true;
	StreamMapIfo->StreamingLevel->SetShouldBeVisible(true);
}

void UMapSteamLoadSystem::ForceRemoveMap(const TSoftObjectPtr<UWorld> Level)
{
	auto* StreamMapIfo =  StreamingLevelMap.Find(Level);
	if (StreamMapIfo == nullptr)return;
	UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(GetWorld(),Level,FLatentActionInfo(),false);	
}
