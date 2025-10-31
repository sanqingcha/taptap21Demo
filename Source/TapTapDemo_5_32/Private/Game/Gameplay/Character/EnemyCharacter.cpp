// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Character/EnemyCharacter.h"

#include "Components/WidgetComponent.h"
#include "Game/Gameplay/Subsytem/GISubSystem/MapSteamLoadSystem.h"
#include "Game/GameplayTags/GameTags.h"
#include "Game/Gas/Attribute/AttributeSetBase.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	/**GasComp*/
	EnemyAbilitySysComp = CreateDefaultSubobject<UAbilitySystemComponent>("GameplayAbility");
	//EnemyDataWidget = CreateDefaultSubobject<UWidgetComponent>("DetialWidget");
	/**eng*/
}
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (!Tags.Contains(Game::Character::Game_Character_Enemy.GetTag().GetTagName()))
	{
		Tags.Add(Game::Character::Game_Character_Enemy.GetTag().GetTagName());
	}
	BindAttributeDelegate();
	UMapSteamLoadSystem* Sys = GetGameInstance()->GetSubsystem<UMapSteamLoadSystem>();
	if (Sys->InBuildSpace){onStop(true);}
	Sys ->OnGetInOrOutSpaceMapDelegate.AddDynamic(this,&AEnemyCharacter::onStop);
}
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AEnemyCharacter::BindAttributeDelegate()
{
	EnemyAbilitySysComp->GetGameplayAttributeValueChangeDelegate(UEnemyAttribute::GetMaxSpeedAttribute()).AddUObject(this,&AEnemyCharacter::OnSpeedChanged);
	EnemyAbilitySysComp->GetGameplayAttributeValueChangeDelegate(UEnemyAttribute::GetHealthAttribute()).AddUObject(this,&AEnemyCharacter::OnHealthChanged);
}

void AEnemyCharacter::OnSpeedChanged(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

void AEnemyCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue <= 0)
	{
		Die();
	}
}

void AEnemyCharacter::onStop(bool Getin)
{
	OnStop_BP(Getin);
}
