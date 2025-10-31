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
	UPROPERTY(BlueprintReadOnly)
	float AmbientVolume = 1.f;
	UPROPERTY(BlueprintReadOnly)
	float DialogueVolume = 1.f;
	UPROPERTY(BlueprintReadOnly)
	float SoundEffectVolume = 1.f;
};

USTRUCT(BlueprintType)
struct FPlayerControlSettings
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly)
	float CameraDelaySpeed = 10.f;
	UPROPERTY(BlueprintReadOnly)
	float CheckTime = 0.1f;
	UPROPERTY(BlueprintReadOnly)
	float TextLag = 5.f;
};

USTRUCT(BlueprintType)
struct FInputSensitivity
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly)
	float RotatorSensitivity = 0.5f;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSoundChanged , const FSoundVolumeSettings&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerControlChanged , const FPlayerControlSettings&);
DECLARE_DELEGATE_OneParam(FOnCheckTimeChanged,float NewCheckTime);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInputSensitivityChanged,const FInputSensitivity& );
UCLASS()
class TAPTAPDEMO_5_32_API UGameSettingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	/**音量*/
	UFUNCTION(BlueprintCallable,Category="SoundSetting|GameSettings")
	const FSoundVolumeSettings& GetSoundSettings(){return SoundSettings;};
	UFUNCTION(BlueprintCallable,Category="SoundSetting|GameSettings")
	void SetSoundVolumeSettings(const FSoundVolumeSettings& NewValue);
	UFUNCTION(BlueprintCallable,Category="SoundSetting|GameSettings")
	void SetAmbientVolume(float NewValoume)	;
	UFUNCTION(BlueprintCallable,Category="SoundSetting|GameSettings")
	void SetDialogueVolume(float NewValue)	;
	UFUNCTION(BlueprintCallable,Category="SoundSetting|GameSettings")
	void SetSoundEffectVolume(float NewValue);
	/**end*/
	
	/**PlayerControl*/
	UFUNCTION(BlueprintCallable,Category="PlayerSetting|GameSettings")
	const FPlayerControlSettings& GetPlayerControlSettings(){return PlayerControlSettings;};
	UFUNCTION(BlueprintCallable,Category="PlayerSetting|GameSettings")
	void SetPlayerControlSettings(const FPlayerControlSettings& NewValue);
	UFUNCTION(BlueprintCallable,Category="Playeretting|GameSettings")
	void SetCameraDelaySpeed(float NewValue);
	UFUNCTION(BlueprintCallable,Category="PlayerSetting|GameSettings")
	void SetTextLag(float NewValue);
	/**end*/
	
	/**MouseSensitivity*/
	UFUNCTION(BlueprintCallable,Category="MouseSetting|GameSettings")
	void SetRotatorSensitivity(float NewValue);
	UFUNCTION(BlueprintCallable,Category="MouseSetting|GameSettings")
	const FInputSensitivity& GetInputSensitivity(){return InputSensitivity;};
	/**end*/
	
	/**BuildSpace的点击/拖拽的检测时间*/
	UFUNCTION(BlueprintCallable,Category="SoundSetting|GameSettings")
	void SetCheckTime(float NewValue);
	UFUNCTION(BlueprintCallable,Category="SoundSetting|GameSettings")
	const float GetCheckTime(){return PlayerControlSettings.CheckTime;}; 
	/***/
	FOnSoundChanged& GetSoundChangedDelegate(){return OnSoundChangedDelegate;};
	FOnPlayerControlChanged& GetPlayerControlChangedDelegate(){return OnPlayerControlChangedDelegate;};
	FOnCheckTimeChanged& GetCheckTimeDelegate(){return OnCheckTimeDelegate;};
	FOnInputSensitivityChanged& GetInputSensitivityDelegate(){return OnInputSensitivityChangedDelegate;};
private:
	FOnSoundChanged OnSoundChangedDelegate;
	FOnPlayerControlChanged OnPlayerControlChangedDelegate;
	FOnCheckTimeChanged OnCheckTimeDelegate;
	FOnInputSensitivityChanged OnInputSensitivityChangedDelegate;
	
	FSoundVolumeSettings SoundSettings;
	FPlayerControlSettings PlayerControlSettings;
	FInputSensitivity InputSensitivity;
};
