// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Subsytem/GISubSystem/MapSteamLoadSystem.h"

#include "Game/Gameplay/Interface/InputMappingInterface.h"
#include "Game/Gameplay/Player/SuperPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"

namespace UMapSteamLoadSystemHelper
{
	static FOnStreamLoadOver OnStreamLoadOver;

}

FStreamMapInfo& UMapSteamLoadSystem::PreLoadLevel(const TSoftObjectPtr<UWorld> Level)
{
	/**移除掉尾部的后缀*/
	const FString AssetPath =  Level.ToString();
	int32 lastIndex = 0;
	check(AssetPath.FindChar('.',lastIndex));
	const FString PathSubStr =  UKismetStringLibrary::GetSubstring(AssetPath, 0,lastIndex); 
	/**end*/
	check(PathSubStr.Len() > 0);
	const FLatentActionInfo LatentInfo(0, FMath::Rand(), TEXT("CompleteOpenMap"), this);
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(),Level,false,false,LatentInfo);
	//LoadStreamMapDelegate.Broadcast(Level);
	
	FStreamMapInfo Data(UGameplayStatics::GetStreamingLevel(GetWorld(), *PathSubStr));
	//Data.StreamingLevel->SetShouldBeVisible(false);
	return StreamingLevelMap.Emplace(Level,Data);
}

void UMapSteamLoadSystem::CompleteOpenMap()
{
	
}

void UMapSteamLoadSystem::OpenMapBySys(const TSoftObjectPtr<UWorld> Level, bool RemovePrevMap, EMapLoadType LoadType)
{
	UWorld* World = GetWorld();
	if (World == nullptr)return;
	if (RemovePrevMap)
	{
		for (auto&i:StreamingLevelMap)
		{
			if (i.Value.HaveLoad&&i.Value.CanbeRemove)
			{
				UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(World,i.Key,FLatentActionInfo(),false);	
			}
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

void UMapSteamLoadSystem::BroadcastOpenLevel()
{
	GetOnStreamLoadOverDelegate().Broadcast();
	OnStreamLoadOverDelegate_BP.Broadcast();
}


FOnStreamLoadOver& UMapSteamLoadSystem::GetOnStreamLoadOverDelegate()
{
	return UMapSteamLoadSystemHelper::OnStreamLoadOver;
}

void UMapSteamLoadSystem::RegisterCamera(const FGameplayTag& Tag, AActor* CameraActor)
{
	if (PlayerCameraMap.Contains(Tag))return;
	PlayerCameraMap.Add(Tag,CameraActor);
}

void UMapSteamLoadSystem::SwitchCamera(APlayerController* PC,const FGameplayTag& Tag,float BlendTime, EViewTargetBlendFunction BlendFunc, float BlendExp, bool bLockOutgoing)
{
	auto TargetCamera = *PlayerCameraMap.Find(Tag);
	if (TargetCamera == nullptr)return;

	if (ASuperPlayerController::PlayerData_Static)
	{
		IInputMappingInterface* TargetCameraInputMapping = Cast<IInputMappingInterface>(TargetCamera);
		if (TargetCameraInputMapping)
		{
			CurrentCamera->DeactivateCamera(ASuperPlayerController::PlayerData_Static);
			TargetCameraInputMapping->ActivateCamera(ASuperPlayerController::PlayerData_Static);
			CurrentCamera = TargetCameraInputMapping;
			PC->SetViewTargetWithBlend(TargetCamera,BlendTime,BlendFunc,BlendExp,bLockOutgoing);
		}
	}
}

ULevelStreaming* UMapSteamLoadSystem::GetCurrentWorldbySoftRef(const TSoftObjectPtr<UWorld> Level)
{
	auto* FindWorld = StreamingLevelMap.Find(Level);
	if (FindWorld == nullptr)return nullptr;
	return FindWorld->StreamingLevel;
}



