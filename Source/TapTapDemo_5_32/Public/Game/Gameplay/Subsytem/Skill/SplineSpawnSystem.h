// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SplineSpawnSystem.generated.h"

class ISkillNodeInteractInterface;
struct FNodeConnectData;
enum class ESplineConnectType : uint8;
class UNodeSplineComponent;
class INodeSplineInterface;
/**
 * 
 */
class ASplineVisualizeActor;

//DECLARE_DELEGATE_OneParam(FOnSplinesSpawn, UNodeSplineComponent*);

UCLASS()
class TAPTAPDEMO_5_32_API USplineSpawnSystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	/**初始化*/
	void RegisterSplineActor(ASplineVisualizeActor* inSplineVisActor);
	/**end*/
	
	UNodeSplineComponent* TryConnect(ISkillNodeInteractInterface* Head,ISkillNodeInteractInterface* Tail,bool& Success);
private:
	void ConnectNode();
public:
	/**end*/

	//FOnSplinesSpawn OnSplinesSpawnDelegate;
private:
	TObjectPtr<ASplineVisualizeActor> SplineVisActor;
};
