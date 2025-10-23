// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Subsytem/GISubSystem/SoundSettingSubsystem.h"

void USoundSettingSubsystem::SetAmbientVolume(float NewValoume)
{
	SoundSettings.AmbientVolume = NewValoume;
	OnSoundChangedDelegate.Broadcast(SoundSettings);
}

void USoundSettingSubsystem::SetDialogueVolume(float NewValue)
{
	SoundSettings.DialogueVolume = NewValue;
	OnSoundChangedDelegate.Broadcast(SoundSettings);
}

void USoundSettingSubsystem::SetSoundEffectVolume(float NewValue)
{
	SoundSettings.SoundEffectVolume = NewValue;
	OnSoundChangedDelegate.Broadcast(SoundSettings);
}
