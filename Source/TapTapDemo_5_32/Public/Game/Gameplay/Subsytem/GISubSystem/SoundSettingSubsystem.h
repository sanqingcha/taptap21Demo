// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SoundSettingSubsystem.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSoundVolumeSettings
{
	GENERATED_BODY()
	float AmbientVolume = 1.f;
	float DialogueVolume = 1.f;
	float SoundEffectVolume = 1.f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSoundChanged , const FSoundVolumeSettings& ,SoundSetting);


UCLASS()
class TAPTAPDEMO_5_32_API USoundSettingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,Category="SoundSetting")
	const FSoundVolumeSettings& GetSoundSettings(){return SoundSettings;};
	UFUNCTION(BlueprintCallable,Category="SoundSetting")
	FSoundVolumeSettings& GetMutSoundSettings(){return SoundSettings;};
	UFUNCTION(BlueprintCallable,Category="SoundSetting")
	void SetSoundVolumeSettings(const FSoundVolumeSettings& NewValue){SoundSettings = NewValue;};
	UFUNCTION(BlueprintCallable,Category="SoundSetting")
	void SetAmbientVolume(float NewValoume)	;
	UFUNCTION(BlueprintCallable,Category="SoundSetting")
	void SetDialogueVolume(float NewValue)	;
	UFUNCTION(BlueprintCallable,Category="SoundSetting")
	void SetSoundEffectVolume(float NewValue);

	
	UPROPERTY(BlueprintAssignable,BlueprintReadWrite)
	FOnSoundChanged	OnSoundChangedDelegate;
private:
	/**环境音量*/
	FSoundVolumeSettings SoundSettings;
};
