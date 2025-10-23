// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SpatialPathFindingResult.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct TAPTAPDEMO_5_32_API FSpatialPathFindingResult 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Direction; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Success;
	FSpatialPathFindingResult();
	~FSpatialPathFindingResult();
};
