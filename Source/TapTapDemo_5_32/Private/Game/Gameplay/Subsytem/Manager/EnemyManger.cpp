// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Subsytem/Manager/EnemyManger.h"

#include "Game/Gameplay/Character/EnemyCharacter.h"

AEnemyCharacter* UEnemyManger::SpawnEnemy(TSubclassOf<AEnemyCharacter> EnemyClass,const FVector Pos)
{
	 GetWorld()->SpawnActor(EnemyClass,&Pos);
	return nullptr;
}

void UEnemyManger::CallAllEnemyShow()
{
	for (auto& i : Enemies)
	{
		i->ShowTips();
	}
}
