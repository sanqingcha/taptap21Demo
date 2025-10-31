// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/DefaultWidget/SettingUserWidget.h"

#include "InputActionValue.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Game/Gameplay/Player/SuperPlayerController.h"
#include "Game/Gameplay/Subsytem/GISubSystem/SoundSettingSubsystem.h"



void USettingUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	/**MenuMainButton*/
	Return_Button->OnPressed.AddDynamic(this,&USettingUserWidget::OnReturnButtonPressed);
		
	
	SettingSystem = GetGameInstance()->GetSubsystem<UGameSettingSubsystem>();
	
	/**音量*/
	const FSoundVolumeSettings& SoundSetting = SettingSystem->GetSoundSettings(); 
	AmbientVolume_Slider->SetValue(SoundSetting.AmbientVolume);
	DialogueVolume_Slider->SetValue(SoundSetting.DialogueVolume);
	SoundEffectVolume_Slider->SetValue(SoundSetting.SoundEffectVolume);
	
	check(SettingSystem);
	AmbientVolume_Slider->OnValueChanged.AddDynamic(this,&USettingUserWidget::OnAmbientVolumeSliderChanged);
	DialogueVolume_Slider->OnValueChanged.AddDynamic(this,&USettingUserWidget::OnDialogueVolumeSliderChanged);
	SoundEffectVolume_Slider->OnValueChanged.AddDynamic(this,&USettingUserWidget::OnSoundEffectVolumeSliderChanged);

	/**角色控制*/
	const FPlayerControlSettings& PlayerControlSettings = SettingSystem->GetPlayerControlSettings();
	CameraLagSpeed_Text->SetText(FText::AsNumber(PlayerControlSettings.CameraDelaySpeed));
	CheckTime_Slider->SetValue(PlayerControlSettings.CheckTime*2);
	TextLagSpeed_Text->SetText(FText::AsNumber(PlayerControlSettings.TextLag));
	
	AddCameraLagSpeed_Bt->OnPressed.AddDynamic(this,&USettingUserWidget::AddCameraLagSpeed_Bt_Pressed);
	ReduceCameraLagSpeed_Bt->OnPressed.AddDynamic(this,&USettingUserWidget::ReduceCameraLagSpeed_Bt_Pressed);
	CheckTime_Slider->OnValueChanged.AddDynamic(this,&USettingUserWidget::OnCheckTimeSliderChanged);

	AddTextLagSpeed_Bt->OnPressed.AddDynamic(this,&USettingUserWidget::AddTextLagSpeed_Bt_Pressed);
	ReduceTextLagSpeed_Bt->OnPressed.AddDynamic(this,&USettingUserWidget::ReduceTextLagSpeed_Bt_Pressed);
	
	/**inputSensitivity*/
	const FInputSensitivity& InputSensitivity = SettingSystem->GetInputSensitivity();
	RotSensitivity_Slider->SetValue(InputSensitivity.RotatorSensitivity);

	RotSensitivity_Slider->OnValueChanged.AddDynamic(this,&USettingUserWidget::OnRotSensitivityChanged);

	
}

void USettingUserWidget::OnReturnButtonPressed()
{
	if (OnReturnButtonDelegate.IsBound())
	{
		OnReturnButtonDelegate.Execute(FInputActionValue());
	}
}

void USettingUserWidget::OnCheckTimeSliderChanged(float Percent)
{
	SettingSystem->SetCheckTime(Percent*0.5);
}

void USettingUserWidget::OnAmbientVolumeSliderChanged(float Percent)
{
	SettingSystem->SetAmbientVolume(Percent);
	//UKismetSystemLibrary::PrintString(GetWorld(),FString::Printf(TEXT("AmbientVolume = %f"), Percent),true,false,FLinearColor::Red,10.f);
}

void USettingUserWidget::OnDialogueVolumeSliderChanged(float Percent)
{
	SettingSystem->SetDialogueVolume(Percent);
	//UKismetSystemLibrary::PrintString(GetWorld(),FString::Printf(TEXT("DilaogeVolume = %f"), Percent),true,false,FLinearColor::Red,10.f);
}

void USettingUserWidget::OnSoundEffectVolumeSliderChanged(float Percent)
{
	SettingSystem->SetSoundEffectVolume(Percent);
	//UKismetSystemLibrary::PrintString(GetWorld(),FString::Printf(TEXT("EffectVolume = %f"), Percent),true,false,FLinearColor::Red,10.f);
}

void USettingUserWidget::AddCameraLagSpeed_Bt_Pressed()
{
	const FPlayerControlSettings& PlayerControlSettings = SettingSystem->GetPlayerControlSettings();
	if (PlayerControlSettings.CameraDelaySpeed+1>50)return;
	SettingSystem->SetCameraDelaySpeed(PlayerControlSettings.CameraDelaySpeed+1);
	CameraLagSpeed_Text->SetText(FText::AsNumber(PlayerControlSettings.CameraDelaySpeed));
}

void USettingUserWidget::ReduceCameraLagSpeed_Bt_Pressed()
{
	const FPlayerControlSettings& PlayerControlSettings = SettingSystem->GetPlayerControlSettings();
	if (PlayerControlSettings.CameraDelaySpeed-1<0) return;
	SettingSystem->SetCameraDelaySpeed(PlayerControlSettings.CameraDelaySpeed-1);
	CameraLagSpeed_Text->SetText(FText::AsNumber(PlayerControlSettings.CameraDelaySpeed));
}

void USettingUserWidget::AddTextLagSpeed_Bt_Pressed()
{
	const FPlayerControlSettings& PlayerControlSettings = SettingSystem->GetPlayerControlSettings();
	if (PlayerControlSettings.TextLag+1>50)return;
	SettingSystem->SetTextLag(PlayerControlSettings.TextLag+1);
	TextLagSpeed_Text->SetText(FText::AsNumber(PlayerControlSettings.TextLag));
}

void USettingUserWidget::ReduceTextLagSpeed_Bt_Pressed()
{
	const FPlayerControlSettings& PlayerControlSettings = SettingSystem->GetPlayerControlSettings();
	if (PlayerControlSettings.TextLag-1<0) return;
	SettingSystem->SetTextLag(PlayerControlSettings.TextLag-1);
	TextLagSpeed_Text->SetText(FText::AsNumber(PlayerControlSettings.TextLag));
}

void USettingUserWidget::OnRotSensitivityChanged(float NewValue)
{
	SettingSystem->SetRotatorSensitivity(NewValue);
}

