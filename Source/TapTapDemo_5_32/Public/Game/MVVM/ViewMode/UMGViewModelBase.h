// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "UMGViewModelBase.generated.h"


class UPlayerBaseData;
/**
 * 
 */
UCLASS()
class TAPTAPDEMO_5_32_API UUMGViewModelBase : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	UUMGViewModelBase();

	UFUNCTION(BlueprintCallable)
	virtual void InitialViewModel(UPlayerBaseData* inBaseData){BaseData = inBaseData;};

protected:
	TObjectPtr<UPlayerBaseData> BaseData;
};
