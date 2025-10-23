// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SuperGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TAPTAPDEMO_5_32_API USuperGameInstance : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
