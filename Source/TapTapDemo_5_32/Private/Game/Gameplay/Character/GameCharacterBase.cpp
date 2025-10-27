// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Character/GameCharacterBase.h"


AGameCharacterBase::AGameCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}
void AGameCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}
void AGameCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

