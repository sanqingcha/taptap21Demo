// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DialoageAsset.generated.h"

struct FGameplayTagContainer;
struct FGameplayTag;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FOnceDialoageData
{
	GENERATED_BODY()
	/**相对于上一句话，间隔时间延迟多久后再触发*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Dialoage",meta = (ClampMin = "0.01",ClampMax = "3.0"))
	float Delay = 0.01f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Dialoage")
	FString Dialoage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Dialoage")
	TSoftObjectPtr<USoundBase> SoundSoft;

	
	UPROPERTY()
	TObjectPtr<USoundBase> SoundSource;
};


USTRUCT(BlueprintType)
struct FDialoageData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialoage")
	TArray<FOnceDialoageData> Dialoage;

	
	/***这个对话可以打断哪些对话*/
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialoage")
	FGameplayTagContainer BreakKeys;*/
};

UCLASS()
class TAPTAPDEMO_5_32_API UDialoageAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	/*/**标记打断最高权限的对话，被标记的对话可以打断任何对话#1#
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialoage")
	FGameplayTagContainer DefaultBreakKeys;*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialoage")
	TMap<FGameplayTag , FDialoageData> DialoageMapData;
};
