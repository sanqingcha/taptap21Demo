// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemyManger.generated.h"

class AEnemyCharacter;
/**
 * 
 */
UCLASS()
class TAPTAPDEMO_5_32_API UEnemyManger : public UWorldSubsystem
{
	GENERATED_BODY()
	public:
	UFUNCTION(BlueprintCallable,Category="EnemyManager" ,meta = (Keywords = "Spawn"))
	AEnemyCharacter* SpawnEnemy(TSubclassOf<AEnemyCharacter> EnemyClass,const FVector Pos);
	UFUNCTION(BlueprintCallable,Category="EnemyManager" ,meta = (Keywords = "Show"))
	void CallAllEnemyShow();
	
	UPROPERTY()
	TSet<AEnemyCharacter*> Enemies;
};
