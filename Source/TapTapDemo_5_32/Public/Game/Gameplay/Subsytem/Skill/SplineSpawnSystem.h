// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SplineSpawnSystem.generated.h"

struct FNodeConnectData;
enum class ESplineConnectType : uint8;
class UNodeSplineComponent;
class INodeSplineInterface;
/**
 * 
 */
class ASplineVisualizeActor;

UCLASS()
class TAPTAPDEMO_5_32_API USplineSpawnSystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	/**初始化*/
	void RegisterSplineActor(ASplineVisualizeActor* inSplineVisActor);
	/**end*/
	
	/**外部调用接口*/
	UFUNCTION(BlueprintCallable)
	void SpawnSpline(const FNodeConnectData& ConnectHead,const FNodeConnectData& ConnectTail);
	/**end*/
	
private:
	
	TObjectPtr<ASplineVisualizeActor> SplineVisActor;
};
