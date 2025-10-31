// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MapSteamLoadSystem.generated.h"


struct FMapAssetData;
class UGameSettingSubsystem;
struct FSoundVolumeSettings;
class UMapSingleAsset;
class IInputMappingInterface;

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStreamLoadOver);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStreamLoadOver_BP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMapChanged,const FMapAssetData& , Param);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGetInOrOutSpaceMap,bool , Getin);

UCLASS()
class TAPTAPDEMO_5_32_API UMapSteamLoadSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta = (AdvancedDisplay = "MapSoftRef"))
	void InitialMapBGM(UMapSingleAsset* MapSingleData,TSoftObjectPtr<UWorld> MapSoftRef);
private:
	void OnSoundChanged(const FSoundVolumeSettings& Setting);
	UPROPERTY()
	TObjectPtr<UMapSingleAsset> MapDatas;
	UGameSettingSubsystem* GameSettingSubSys;
	
	FMapAssetData* LastData;
public:
	
	UFUNCTION(BlueprintCallable,category="MapSteamLoadSystem",meta = (Keywords = "Load,Stream,Open"))
	FStreamMapInfo& PreLoadLevel(const TSoftObjectPtr<UWorld> Level);
	UFUNCTION()
	void CompleteOpenMap();
	UFUNCTION(BlueprintCallable,category="MapSteamLoadSystem",meta = (RemovePrevMap = "true",Keywords = "Load,Stream,Open",AdvancedDisplay = "RemovePrevMap,LoadType"))
	void OpenMapBySys(const TSoftObjectPtr<UWorld> Level,bool RemovePrevMap, EMapLoadType LoadType);
	UFUNCTION(BlueprintCallable,category="MapSteamLoadSystem",meta = (Keywords = "Load,Stream,Close"))
	void ForceRemoveMap(const TSoftObjectPtr<UWorld> Level);
	UFUNCTION(BlueprintCallable,category="MapSteamLoadSystem")
	void BroadcastOpenLevel();

	static FOnStreamLoadOver& GetOnStreamLoadOverDelegate();
	static FOnMapChanged& GetOnMapChangedDelegate();

	UPROPERTY(BlueprintAssignable,BlueprintReadWrite)
	FOnStreamLoadOver_BP OnStreamLoadOverDelegate_BP;
	TMap<TSoftObjectPtr<UWorld>,FStreamMapInfo> StreamingLevelMap;


	UFUNCTION(BlueprintCallable)
	void RegisterCamera(const FGameplayTag& Tag,AActor* CameraActor);

	UFUNCTION(BlueprintCallable)
	void SwitchCamera(APlayerController* PC,const FGameplayTag& Tag,float BlendTime, EViewTargetBlendFunction BlendFunc, float BlendExp, bool bLockOutgoing);
	void SetCurrentCamera(IInputMappingInterface* inCurrentCamera){CurrentCamera = inCurrentCamera;}
	bool InBuildSpace = false;
private:
public:
	UFUNCTION(BlueprintCallable)
	ULevelStreaming* GetCurrentWorldbySoftRef(const TSoftObjectPtr<UWorld> Level);

	UPROPERTY(BlueprintAssignable,BlueprintReadWrite)
	FOnGetInOrOutSpaceMap OnGetInOrOutSpaceMapDelegate;
private:
	IInputMappingInterface* CurrentCamera;
	TMap<FGameplayTag,AActor* > PlayerCameraMap;
};
