// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineMeshComponent.h"
#include "NodeSplineMeshComp.generated.h"

/**
 * 
 */
UCLASS()
class TAPTAPDEMO_5_32_API UNodeSplineMeshComp : public USplineMeshComponent
{
	GENERATED_BODY()
public:
	UNodeSplineMeshComp();
	virtual void SetPoolState(bool isVis);
};
