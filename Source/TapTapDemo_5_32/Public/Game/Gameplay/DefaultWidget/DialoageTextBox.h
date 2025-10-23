// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialoageTextBox.generated.h"

/**
 * 
 */
UCLASS()
class TAPTAPDEMO_5_32_API UDialoageTextBox : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	void AddTextWidget();
};
