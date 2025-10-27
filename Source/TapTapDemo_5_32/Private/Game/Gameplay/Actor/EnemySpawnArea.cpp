// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Actor/EnemySpawnArea.h"

#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryOption.h"
#include "EnvironmentQuery/Generators/EnvQueryGenerator_SimpleGrid.h"
#include "Game/Gameplay/Character/EnemyCharacter.h"
#include "Game/Gameplay/Subsytem/GISubSystem/MapSteamLoadSystem.h"
#include "Game/GameplayTags/GameTags.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

AEnemySpawnArea::AEnemySpawnArea()
{
	PrimaryActorTick.bCanEverTick = false;

	DebugMeshComp = CreateDefaultSubobject<UMeshComponent>(TEXT("DebugMeshComp"));
	
}

void AEnemySpawnArea::BeginPlay()
{
	Super::BeginPlay();
	if (EnemyClass.IsEmpty())Destroy();
	
#ifndef IF_WITH_EDITOR
	DebugMeshComp->SetVisibility(false);
#endif
	
}

void AEnemySpawnArea::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
}

void AEnemySpawnArea::ActivateSpawn()
{
	if (Timer.IsValid())
	{
		GetWorld()->GetTimerManager().UnPauseTimer(Timer);
		return;
	}
	GetWorld()->GetTimerManager().SetTimer(
		Timer,
		this,
		&AEnemySpawnArea::Check,
		SpawnSpeed,
		true
		);
}

void AEnemySpawnArea::DeactivateSpawn()
{
	GetWorld()->GetTimerManager().PauseTimer(Timer);
}

void AEnemySpawnArea::Check()
{
	for (int32 i = SpawnedEnemys.Num() - 1; i >= 0; i--)
	{
		if (IsValid(SpawnedEnemys[i]))
		{
			continue;
		}
		SpawnedEnemys.RemoveAtSwap(i);
	}
	if (SpawnedEnemys.Num() < MaxCount)
	{
		TrySpawnOnce();
	}
}

void AEnemySpawnArea::TrySpawnOnce()
{
	if (!ensure(QueryAsset))return;
	UObject* OwnerStreamMap = nullptr;
	UMapSteamLoadSystem* StreamMapManager = GetWorld()->GetGameInstance()->GetSubsystem<UMapSteamLoadSystem>();
	if (StreamMapManager)
	{
		OwnerStreamMap = StreamMapManager->GetCurrentWorldbySoftRef(OwnerWorld);
	}
	if (OwnerStreamMap==nullptr)
	{
		OwnerStreamMap = GetWorld();
	}
	if (OwnerStreamMap)
	{
		UEnvQueryInstanceBlueprintWrapper* QueryWrapper =  UEnvQueryManager::RunEQSQuery(OwnerStreamMap,QueryAsset,this,EEnvQueryRunMode::Type::RandomBest5Pct,nullptr);
		if (QueryWrapper)
		{
			QueryWrapper->GetOnQueryFinishedEvent().AddDynamic(this,&AEnemySpawnArea::OnQueryCompleted);
		}
	}
}

void AEnemySpawnArea::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,EEnvQueryStatus::Type QueryStatus)
{
	TArray<FVector> ResultLocations;
	QueryInstance->GetQueryResultsAsLocations(ResultLocations);
	if (ResultLocations.Num() > 0)
	{
		for (int i = 0; i < MaxCount; i++)
		{
			if (ResultLocations.IsValidIndex(i))
			{
				SpawnEnemy(ResultLocations[i]);
			}
			else return;
		}
	}
}

void AEnemySpawnArea::SpawnEnemy(const FVector& Location)
{
	int32 RandomIndex = FMath::RandRange(0,EnemyClass.Num()-1);
	if (EnemyClass[RandomIndex].GetDefaultObject()->ActorHasTag(Game::Character::Game_Character_Enemy.GetTag().GetTagName()))
	{
		DrawDebugSphere(GetWorld(),Location+FVector(0,0,100),100,12,FColor::Red,true,100);
		AActor* Enemy = GetWorld()->SpawnActorDeferred<AActor>(EnemyClass[RandomIndex],FTransform(Location+FVector(0,0,100)));
		Enemy->FinishSpawning(FTransform(Location+FVector(0,0,100)));
		SpawnedEnemys.Add(Enemy);
	}
}

void AEnemySpawnArea::test(FVector Location)
{
	// 获取查询的所有选项
	TArray<UEnvQueryOption*>& Options = QueryAsset->GetOptionsMutable();
	for (UEnvQueryOption* Option : Options)
	{
		if (Option && Option->Generator)
		{
			// 检查是否为 Grid 生成器
			UEnvQueryGenerator_SimpleGrid* GridGen = Cast<UEnvQueryGenerator_SimpleGrid>(Option->Generator);
			if (GridGen)
			{
				// 动态设置 Grid 半径
				const FVector & Scale = GetActorScale();  
				GridGen->GridSize.DefaultValue = FMath::Min(Scale.X, Scale.Y)*25;
				UE_LOG(LogTemp,Error,TEXT("Size = %s") , *GridGen->GridSize.ToString() );
				break;
			}
		}
	}
}

