// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialoageTrriggerEventBase.generated.h"

class UPlayerBaseData;
/**
 * 
 */
UCLASS(Blueprintable)
class TAPTAPDEMO_5_32_API UDialoageTrriggerEventBase : public UObject
{
	GENERATED_BODY()
	public:
	virtual void OnTrrigger(UPlayerBaseData* PlayerData);
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnTrriggerBP(UPlayerBaseData* PlayerData);
	
	UFUNCTION(BlueprintCallable, Category = "Dialoage")
	virtual void EndTrrigger();
	
};

UCLASS()
class TAPTAPDEMO_5_32_API UDialoageTrriggerEvent_LoadBuildSpace final:  public UDialoageTrriggerEventBase
{
	GENERATED_BODY()
	public:
	virtual void OnTrrigger(UPlayerBaseData* PlayerData) override;
};
