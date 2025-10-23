// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ViewModeUserWidget.generated.h"

class UPlayerBaseData;
class UUMGViewModelBase;
/**
 * 
 */
UCLASS()
class TAPTAPDEMO_5_32_API UViewModeUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void Initial(UPlayerBaseData* inBaseData){BaseData = inBaseData;};
	UFUNCTION(BlueprintImplementableEvent)
	void InitialBP();
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPlayerBaseData> BaseData;
};

UCLASS()
class TAPTAPDEMO_5_32_API UPlayerAttributeWidget : public UUserWidget
{
	GENERATED_BODY()
};

UCLASS()
class TAPTAPDEMO_5_32_API UEnemyAttributeWidget : public UUserWidget
{
	GENERATED_BODY()
};

