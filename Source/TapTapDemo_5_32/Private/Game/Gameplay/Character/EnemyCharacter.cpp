// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Character/EnemyCharacter.h"

#include "Game/GameplayTags/GameTags.h"
#include "Game/Gas/Attribute/AttributeSetBase.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	/**GasComp*/
	EnemyAbilitySysComp = CreateDefaultSubobject<UAbilitySystemComponent>("GameplayAbility");
	/**eng*/
}
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (!Tags.Contains(Game::Character::Game_Character_Enemy.GetTag().GetTagName()))
	{
		Tags.Add(Game::Character::Game_Character_Enemy.GetTag().GetTagName());
	}
}
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
