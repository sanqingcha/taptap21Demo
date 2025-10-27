// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlayerBaseData.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
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
	APlayerController* PC = nullptr;
	UPROPERTY(BlueprintReadOnly)
	APlayerState* PS = nullptr;
	UPROPERTY(BlueprintReadOnly)
	UAbilitySystemComponent* ASC = nullptr;
	UPROPERTY(BlueprintReadOnly)
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSys;
	UPROPERTY(BlueprintReadOnly)
	UEnhancedInputComponent* EnhancedInputComp;
	
	void SetData(APlayerController*inPC,
		APlayerState* inPS,
		UAbilitySystemComponent* inASC,
		UEnhancedInputLocalPlayerSubsystem* inEnhancedInputSys,
		UEnhancedInputComponent* inEnhancedInputComp)
	{		
		PC = inPC;
		PS = inPS;
		ASC = inASC;
		EnhancedInputSys = inEnhancedInputSys;
		EnhancedInputComp = inEnhancedInputComp;

		check(PC);
		check(PS);
		check(ASC);
		check(EnhancedInputSys);
		check(EnhancedInputComp);
	}
};
