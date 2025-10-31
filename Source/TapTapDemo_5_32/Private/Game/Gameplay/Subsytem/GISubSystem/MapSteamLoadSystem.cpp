// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Subsytem/GISubSystem/MapSteamLoadSystem.h"

#include "Components/AudioComponent.h"
#include "Game/Gameplay/Interface/InputMappingInterface.h"
#include "Game/Gameplay/Player/SuperPlayerController.h"
#include "Game/Gameplay/Subsytem/GISubSystem/SoundSettingSubsystem.h"
#include "Game/Gameplay/Subsytem/GISubSystem/Data/MapSingleAsset.h"
#include "Game/Gameplay/Subsytem/Skill/SkillsRuntimeSubsystem.h"
#include "Game/GameplayTags/GameTags.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"

namespace UMapSteamLoadSystemHelper
{
	static FOnStreamLoadOver OnStreamLoadOver;
	static FOnMapChanged OnMapChangedDelegate;

}

void UMapSteamLoadSystem::InitialMapBGM(UMapSingleAsset* MapSingleData, TSoftObjectPtr<UWorld> MapSoftRef)
{
	GameSettingSubSys = GetGameInstance()->GetSubsystem<UGameSettingSubsystem>();
	if (ensure(GameSettingSubSys))
	{
		if (!MapSingleData)return;
		MapDatas = MapSingleData;
		if (MapSoftRef.IsNull())return;
		auto* Data = MapDatas->MapsAssetData.Find(MapSoftRef);
		if (Data == nullptr)return;
		if (Data->SoundSourceSoftRef.IsNull())return;
		Data->LoadSound();
		Data->SoundComp = UGameplayStatics::SpawnSound2D(
					GetWorld(),
					Data->SoundPtr,
					GameSettingSubSys->GetSoundSettings().AmbientVolume);
		GameSettingSubSys->GetSoundChangedDelegate().AddUObject(this,&UMapSteamLoadSystem::OnSoundChanged);
		LastData = Data;
		Data->isPlaying = true;
	}
}

void UMapSteamLoadSystem::OnSoundChanged(const FSoundVolumeSettings& Setting)
{
	for (auto& i:MapDatas->MapsAssetData)
	{
		if (i.Value.isPlaying&&i.Value.SoundPtr)
		{
			i.Value.SoundComp->SetVolumeMultiplier(Setting.AmbientVolume);
		}
	}
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
	
	auto* StreamMapIfo =  StreamingLevelMap.Find(Level);
	if (StreamMapIfo)
	{
		if (StreamMapIfo->HaveLoad)return;
	}
	UWorld* World = GetWorld();
	if (World == nullptr)return;
	if (RemovePrevMap)
	{
		TArray<TSoftObjectPtr<UWorld>> RemoveMap;
		for (auto&i:StreamingLevelMap)
		{
			if (i.Value.HaveLoad&&i.Value.CanbeRemove)
			{
				UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(World,i.Key,FLatentActionInfo(),false);
				RemoveMap.Add(i.Key);
				i.Value.HaveLoad = false;
			}
		}
		for (auto& i:RemoveMap)
		{
			StreamingLevelMap.Remove(i);
		}
	}
	if (StreamMapIfo == nullptr)
	{
		StreamMapIfo = &PreLoadLevel(Level);
	}
	StreamMapIfo->CanbeRemove = (LoadType == EMapLoadType::Default);
	StreamMapIfo->HaveLoad = true;
	if (StreamMapIfo->StreamingLevel)
	{
		StreamMapIfo->StreamingLevel->SetShouldBeVisible(true);
	}
	auto* Data = MapDatas->MapsAssetData.Find(Level);
	if (Data == nullptr)return;
	if (Data->SoundSourceSoftRef.IsNull())return;
	Data->LoadSound();
	Data->SoundComp = UGameplayStatics::SpawnSound2D(
				GetWorld(),
				Data->SoundPtr,
				GameSettingSubSys->GetSoundSettings().AmbientVolume);
	Data->isPlaying = true;
	GameSettingSubSys->GetSoundChangedDelegate().AddUObject(this,&UMapSteamLoadSystem::OnSoundChanged);
	if (LastData&& LastData!=Data)
	{
		FMapAssetData* SoundLast = LastData;
		LastData->isPlaying = false;
		FTimerHandle handle;
		FTimerDelegate Delegate;
		const float CurrVolume = GameSettingSubSys->GetSoundSettings().AmbientVolume;
		Delegate.BindLambda([SoundLast,this,handle]()
		{
			if (!SoundLast->SoundComp)return;
			const float RemainTime = GetWorld()->GetTimerManager().GetTimerRemaining(handle);
			SoundLast->SoundComp->SetVolumeMultiplier(RemainTime);
			if (RemainTime <= 0.0625)
			{
				SoundLast->SoundComp->Stop();
				SoundLast->SoundComp = nullptr;
			}
		});
		GetWorld()->GetTimerManager().SetTimer(
			handle,
			Delegate,
			CurrVolume,
			false);
	}
	GetOnMapChangedDelegate().Broadcast(*Data);
	LastData = Data;
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

FOnMapChanged& UMapSteamLoadSystem::GetOnMapChangedDelegate()
{
	return UMapSteamLoadSystemHelper::OnMapChangedDelegate;
}

void UMapSteamLoadSystem::RegisterCamera(const FGameplayTag& Tag, AActor* CameraActor)
{
	if (PlayerCameraMap.Contains(Tag))return;
	PlayerCameraMap.Add(Tag,CameraActor);
}

void UMapSteamLoadSystem::SwitchCamera(APlayerController* PC,const FGameplayTag& Tag,float BlendTime, EViewTargetBlendFunction BlendFunc, float BlendExp, bool bLockOutgoing)
{
	const FGameplayTag* TagPtr = &Tag;
	if (!ensure(TagPtr))return;
	auto TargetCamera = *PlayerCameraMap.Find(Tag);
	if (TargetCamera == nullptr)return;

	bool NeedGetin = Tag==Game::Camera::Game_Camera_BuildSpace;
	if ( NeedGetin!=InBuildSpace)
	{
		InBuildSpace = NeedGetin;
		OnGetInOrOutSpaceMapDelegate.Broadcast(InBuildSpace);
		if (InBuildSpace)
		{
			GetGameInstance()->GetSubsystem<USkillsRuntimeSubsystem>()->StopAllStartNode();
		}
		else
		{
			GetGameInstance()->GetSubsystem<USkillsRuntimeSubsystem>()->LaunchAllStartNode();
		}
	}
	
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



