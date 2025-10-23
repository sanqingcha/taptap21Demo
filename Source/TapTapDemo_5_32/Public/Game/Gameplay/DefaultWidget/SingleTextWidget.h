// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SingleTextWidget.generated.h"

/**
 * 
 */
class UPlayerDialoageWidget;
UCLASS()
class TAPTAPDEMO_5_32_API USingleTextWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	/***/
	void SetText(const FString& Text,bool isLast,UPlayerDialoageWidget* TextOwner);
	UFUNCTION(BlueprintImplementableEvent)
	void SetTextBP(const FString& Text);
	UFUNCTION(BlueprintNativeEvent)
	void ReadOverToFall();
	UFUNCTION(BlueprintCallable)
	void AnimEndCallback();
	
	UPROPERTY(BlueprintReadOnly)
	bool NeedReCall = false;
	UPlayerDialoageWidget* Owner = nullptr;
};
