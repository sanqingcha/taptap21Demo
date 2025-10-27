// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SkillNodeInteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USkillNodeInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TAPTAPDEMO_5_32_API ISkillNodeInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void StartTouching() = 0;
	virtual void EndTouching() = 0;
	virtual void StartHolding() = 0;
	virtual void EndHolding() = 0;
};
