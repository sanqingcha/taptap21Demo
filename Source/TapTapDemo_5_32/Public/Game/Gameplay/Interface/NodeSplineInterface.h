// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NodeSplineInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNodeSplineInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TAPTAPDEMO_5_32_API INodeSplineInterface
{
	GENERATED_BODY()
public:
	virtual void SetSpline(const struct  FSplineConnectData& Data,uint32 Hash/**使用连接点的偏移位置计算的Hash*/) = 0;
	virtual void RemoveSpline(const uint32 ConnectPosHash) = 0;
};
