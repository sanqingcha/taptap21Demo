// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SkillNodeWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USkillNodeWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TAPTAPDEMO_5_32_API ISkillNodeWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void ActivateDetial() ;
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void DeactivateDetial();
};
