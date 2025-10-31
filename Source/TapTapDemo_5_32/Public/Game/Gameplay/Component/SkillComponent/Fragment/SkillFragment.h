// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/Gameplay/Skills/SkillType.h"
#include "UObject/Object.h"
#include "SkillFragment.generated.h"

class UPlayerBaseData;
/**
 *
 */
UCLASS(Blueprintable)
class TAPTAPDEMO_5_32_API USkillFragment : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnTrrigger(UPlayerBaseData *Data, const FVector &Trrigger, const FNodeBuffValueFinal &Type, AActor *Source);
};

/**例子*/
UCLASS(Blueprintable)
class TAPTAPDEMO_5_32_API USkillFragment_ApplyDatageRange : public USkillFragment
{
	GENERATED_BODY()
public:
	virtual void OnTrrigger_Implementation(UPlayerBaseData *Data, const FVector &Trrigger, const FNodeBuffValueFinal &Type, AActor *Source) override;
};
