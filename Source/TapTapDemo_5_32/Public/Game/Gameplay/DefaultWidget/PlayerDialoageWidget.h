// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game/Gameplay/Interface/DialoageWidgetInterface.h"
#include "PlayerDialoageWidget.generated.h"

struct FSoundVolumeSettings;
class UGameSettingSubsystem;
class USingleTextWidget;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReadOverOnceDialoage);/**蓝图中调用,用于回调，通知一条字幕播放完*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBreakDialoageRightNow);

UCLASS()
class TAPTAPDEMO_5_32_API UPlayerDialoageWidget : public UUserWidget ,public IDialoageWidgetInterface
{
	GENERATED_BODY()
public:
	/**蓝图中调用,用于回调*/
	UPROPERTY(BlueprintAssignable,BlueprintReadWrite)
	FOnReadOverOnceDialoage OnReadOverOnceDelegate;
	/**c++调用，用于立刻打断*/
	UPROPERTY(BlueprintAssignable,BlueprintReadWrite)
	FBreakDialoageRightNow DialoageOverRightNowDelegate;
	
	virtual void NativeConstruct() override;
	virtual void NewDialoage(TArray<FOnceDialoageData>* Dialoage) override;
	
	void ReadOnce();
	void ReadOncePlayAnim(FString& CurrDialoage);
	UFUNCTION()
	void ReadOverCallback();
	/**CallByTimer*/
	void ReadOneText();
	void CreateOneText(const FString& Text,bool isLast);
	UFUNCTION(BlueprintImplementableEvent)
	void CreateOneTextBP(USingleTextWidget* TextWodget);
	UFUNCTION()
	void OnSoundChaned(const FSoundVolumeSettings& SoundSetting);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="DialoageWidgetSettings")
	float ReadSpeed = 0.05;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSoftClassPtr<USingleTextWidget> TextWidgetClass;
protected:
	FTimerHandle ReadTimerHandle;
	TArray<FOnceDialoageData>* DialoageArray;
	int32 CurrIndex = 0;
	
	FString CurrText;
	int32 CurrTextIndex;
	bool isDelaying = false;
	UPROPERTY()
	UAudioComponent* LastSound = nullptr;
	UGameSettingSubsystem* GameSettingSubsys;

	
};
