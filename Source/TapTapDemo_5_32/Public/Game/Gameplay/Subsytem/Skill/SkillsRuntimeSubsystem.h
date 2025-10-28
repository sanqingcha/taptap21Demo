// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SkillsRuntimeSubsystem.generated.h"

class USkillNode;


/**
 * 
 */
UCLASS()
class TAPTAPDEMO_5_32_API USkillsRuntimeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<USkillNode*> StartSkillNodes;
};
