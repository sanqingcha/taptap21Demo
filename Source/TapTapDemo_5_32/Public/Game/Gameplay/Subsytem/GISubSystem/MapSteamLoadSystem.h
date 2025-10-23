// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MapSteamLoadSystem.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMapLoadType :uint8
{
	Default,
	Infinite,
};


USTRUCT(BlueprintType)
struct FStreamMapInfo
{
	GENERATED_BODY()
	FStreamMapInfo() = default;
	FStreamMapInfo(ULevelStreaming* StreamMap):StreamingLevel(StreamMap){}
	bool CanbeRemove = true;
	bool HaveLoad = false;
	ULevelStreaming*  StreamingLevel = nullptr;
};

UCLASS()
class TAPTAPDEMO_5_32_API UMapSteamLoadSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,category="MapSteamLoadSystem",meta = (Keywords = "Load,Stream,Open"))
	FStreamMapInfo& PreLoadLevel(const TSoftObjectPtr<UWorld> Level);
	
	UFUNCTION(BlueprintCallable,category="MapSteamLoadSystem",meta = (RemovePrevMap = "true",Keywords = "Load,Stream,Open",AdvancedDisplay = "RemovePrevMap,LoadType"))
	void OpenMapBySys(const TSoftObjectPtr<UWorld> Level,bool RemovePrevMap, EMapLoadType LoadType);
	UFUNCTION(BlueprintCallable,category="MapSteamLoadSystem",meta = (Keywords = "Load,Stream,Close"))
	void ForceRemoveMap(const TSoftObjectPtr<UWorld> Level);

	TMap<TSoftObjectPtr<UWorld>,FStreamMapInfo> StreamingLevelMap;
};
