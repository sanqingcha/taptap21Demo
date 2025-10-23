// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "SkillInputAsset.generated.h"

class UInputAction;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FSkillInputData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInputAction> SkillInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkillModuleName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETriggerEvent TrriggerType = ETriggerEvent::Started;
	
};


UCLASS()
class TAPTAPDEMO_5_32_API USkillInputAsset : public UDataAsset
{
	GENERATED_BODY()
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag,FSkillInputData> SkillInputData;
};
