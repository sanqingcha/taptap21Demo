// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MapSingleAsset.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FMapAssetData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite,DisplayName="BGM")
	TSoftObjectPtr<USoundBase> SoundSourceSoftRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,DisplayName="StartTransform")
	FTransform MapStartTransform;
	
	USoundBase* LoadSound()
	{
		if (SoundPtr==nullptr)
		{
			SoundPtr = SoundSourceSoftRef.LoadSynchronous();
		}
		return SoundPtr;
	};
	UPROPERTY()
	TObjectPtr<USoundBase> SoundPtr;
	bool isPlaying = false;
	UPROPERTY()
	UAudioComponent* SoundComp ;
};


UCLASS()
class TAPTAPDEMO_5_32_API UMapSingleAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<TSoftObjectPtr<UWorld>,FMapAssetData> MapsAssetData;
};
