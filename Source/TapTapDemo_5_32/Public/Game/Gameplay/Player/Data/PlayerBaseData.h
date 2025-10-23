// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlayerBaseData.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
/**
 * 
 */

UCLASS(Blueprintable)
class TAPTAPDEMO_5_32_API UPlayerBaseData : public UObject
{
	GENERATED_BODY()
	public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<APlayerController> PC = nullptr;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<APlayerState> PS = nullptr;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> ASC = nullptr;

	void SetData(APlayerController*inPC,APlayerState* inPS,UAbilitySystemComponent* inASC)
	{
		PC = inPC;
		PS = inPS;
		ASC = inASC;
	}
};
