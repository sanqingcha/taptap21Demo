// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Subsytem/GISubSystem/SoundSettingSubsystem.h"


void UGameSettingSubsystem::SetSoundVolumeSettings(const FSoundVolumeSettings& NewValue)
{
	SoundSettings = NewValue;
	GetSoundChangedDelegate().Broadcast(SoundSettings);
}

void UGameSettingSubsystem::SetAmbientVolume(float NewValoume)
{
	SoundSettings.AmbientVolume = NewValoume;
	GetSoundChangedDelegate().Broadcast(SoundSettings);
}

void UGameSettingSubsystem::SetDialogueVolume(float NewValue)
{
	SoundSettings.DialogueVolume = NewValue;
	GetSoundChangedDelegate().Broadcast(SoundSettings);
}

void UGameSettingSubsystem::SetSoundEffectVolume(float NewValue)
{
	SoundSettings.SoundEffectVolume = NewValue;
	GetSoundChangedDelegate().Broadcast(SoundSettings);
}

void UGameSettingSubsystem::SetPlayerControlSettings(const FPlayerControlSettings& NewValue)
{
	PlayerControlSettings = NewValue;
	GetPlayerControlChangedDelegate().Broadcast(PlayerControlSettings);
}

void UGameSettingSubsystem::SetCameraDelaySpeed(float NewValue)
{
	PlayerControlSettings.CameraDelaySpeed = NewValue;
	GetPlayerControlChangedDelegate().Broadcast(PlayerControlSettings);
}

void UGameSettingSubsystem::SetCheckTime(float NewValue)
{
	PlayerControlSettings.CheckTime = NewValue;
	if (GetCheckTimeDelegate().IsBound())
	{
		GetCheckTimeDelegate().Execute(NewValue);
	}
}
