// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputMappingInterface.generated.h"



class UPlayerBaseData;

UINTERFACE(MinimalAPI)
class UInputMappingInterface : public UInterface
{
	GENERATED_BODY()
};

class TAPTAPDEMO_5_32_API IInputMappingInterface
{
	GENERATED_BODY()
public:
	virtual void ActivateCamera(UPlayerBaseData* inPlayerData) = 0;
	virtual void DeactivateCamera(UPlayerBaseData* inPlayerData) = 0;
};
