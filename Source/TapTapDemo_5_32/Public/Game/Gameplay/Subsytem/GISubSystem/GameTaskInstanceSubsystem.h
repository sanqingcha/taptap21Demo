// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameTaskInstanceSubsystem.generated.h"

class ITaskObjectInterface;
/**
 * 
 */
UCLASS()
class TAPTAPDEMO_5_32_API UGameTaskInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	public:
	UFUNCTION(BlueprintCallable)
	void RegisterInterface(TScriptInterface<ITaskObjectInterface> TargetInterface);
};
