// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingUserWidget.generated.h"

/**
 * 
 */


struct FInputActionValue;
class UButton;
class UTextBlock;
struct FSoundVolumeSettings;
class UGameSettingSubsystem;
class USlider;

DECLARE_DELEGATE_OneParam(FOnReturnButtonPressed,const FInputActionValue& )

UCLASS()
class TAPTAPDEMO_5_32_API USettingUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	/**MenuMainButton*/
	UFUNCTION()
	void OnReturnButtonPressed();
	
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	UButton* Return_Button;
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	UButton* Option_Button;
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	UButton* Menu_Button;
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	UButton* Exit_Button;
	
	FOnReturnButtonPressed OnReturnButtonDelegate;
	
	
	
	/**SoundVolume*/
	UFUNCTION()
	void OnAmbientVolumeSliderChanged(float Percent);
	UFUNCTION()
	void OnDialogueVolumeSliderChanged(float Percent);
	UFUNCTION()
	void OnSoundEffectVolumeSliderChanged(float Percent);
	
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	USlider* AmbientVolume_Slider;
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	USlider* DialogueVolume_Slider;
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	USlider* SoundEffectVolume_Slider;

	/**PlayerControl*/
	UFUNCTION()
	void AddCameraLagSpeed_Bt_Pressed();
	UFUNCTION()
	void ReduceCameraLagSpeed_Bt_Pressed();
	
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock* CameraLagSpeed_Text;
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UButton* AddCameraLagSpeed_Bt;
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UButton* ReduceCameraLagSpeed_Bt;
	
	
private:
	UGameSettingSubsystem* SettingSystem;
};
