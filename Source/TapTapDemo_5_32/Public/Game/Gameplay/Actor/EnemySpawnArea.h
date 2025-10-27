// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnArea.generated.h"

class AEnemyCharacter;
class UEnvQueryInstanceBlueprintWrapper;
class UEnvQuery;

UCLASS()
class TAPTAPDEMO_5_32_API AEnemySpawnArea : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,meta = (DisplayPriority = 0),Category= "SpawnControl")
	float SpawnSpeed = 1.f;
	UPROPERTY(EditAnywhere,meta = (DisplayPriority = 0),Category= "SpawnControl")
	int32 MaxCount = 3;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UMeshComponent> DebugMeshComp;
	
	AEnemySpawnArea();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(BlueprintCallable)
	void ActivateSpawn();
	UFUNCTION(BlueprintCallable)
	void DeactivateSpawn();
	
	void Check();
	UFUNCTION(BlueprintCallable)
	void TrySpawnOnce();
	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,EEnvQueryStatus::Type QueryStatus);
	void SpawnEnemy(const FVector& Location);

	UFUNCTION(BlueprintCallable)
	void test(FVector Location);
	
	FTimerHandle Timer;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UEnvQuery* QueryAsset;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<TSubclassOf<AActor>> EnemyClass;
	TArray<AActor*> SpawnedEnemys;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSoftObjectPtr<UWorld> OwnerWorld;
};


