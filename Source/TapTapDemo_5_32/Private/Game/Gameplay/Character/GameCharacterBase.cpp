// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Character/GameCharacterBase.h"

#include "Game/Gameplay/Component/SkillComponent/ExecuteSkillComponent.h"


AGameCharacterBase::AGameCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	ExecuteComp = CreateDefaultSubobject<USkillComponent>("ExecuteComp");
}
void AGameCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	BindAttributeDelegate();
}
void AGameCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UCharacterMovementComponent* AGameCharacterBase::GetCharacterMovementComp()
{
	return GetCharacterMovement();
}

