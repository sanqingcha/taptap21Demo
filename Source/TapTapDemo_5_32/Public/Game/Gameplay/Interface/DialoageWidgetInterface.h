// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DialoageWidgetInterface.generated.h"

struct FOnceDialoageData;
struct FDialoageData;
struct FGameplayTag;

UINTERFACE(MinimalAPI)
class UDialoageWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

class TAPTAPDEMO_5_32_API IDialoageWidgetInterface
{
	GENERATED_BODY()
public:
	virtual void NewDialoage(TArray<FOnceDialoageData>* Dialoage) = 0;
};
