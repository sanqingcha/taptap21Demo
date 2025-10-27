// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Character/PlayerCharacter.h"

#include "CborTypes.h"
#include "Camera/CameraComponent.h"
#include "Game/Gameplay/Component/CameraRegisterComponent.h"
#include "Game/Gameplay/Component/DialoageComponent.h"
#include "Game/Gameplay/Subsytem/GISubSystem/MapSteamLoadSystem.h"
#include "Game/Gameplay/Subsytem/GISubSystem/SoundSettingSubsystem.h"
#include "Game/GameplayTags/GameTags.h"
#include "Game/Gas/Attribute/AttributeSetBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	
	/**GasComp*/
	PlayerAbilitySysComp = CreateDefaultSubobject<UAbilitySystemComponent>("GameplayAbility");
	/**eng*/

	/**Camera*/
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(GetRootComponent());
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
	SpringArmComp->bUsePawnControlRotation = true;
	/**end*/

	/**Dialoage*/
	DialoageWidgetRoot = CreateDefaultSubobject<USpringArmComponent>("DialoageWidgetRoot");
	DialoageWidgetRoot->SetupAttachment(CameraComp);
	DialoageComp = CreateDefaultSubobject<UDialoageComponent>("DialoageComp");
	DialoageComp->SetupAttachment(DialoageWidgetRoot);
	
	DialoageWidgetRoot->SetRelativeTransform({});/**KeepZoreTransform*/
	//DialoageComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	/**end*/
	
	CameraRegisterCompoent = CreateDefaultSubobject<UCameraRegisterComponent>(TEXT("CameraRegisterComponent"));
	
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(Game::Character::Game_Character_Player.GetTag().GetTagName());
	
	//PlayerAbilitySysComp->InitAbilityActorInfo(this,this);
	GameSettingSubsys = GetGameInstance()->GetSubsystem<UGameSettingSubsystem>();
	if (ensure(GameSettingSubsys))
	{
		GameSettingSubsys->SetCameraDelaySpeed(SpringArmComp->CameraLagSpeed);
		GameSettingSubsys->GetPlayerControlChangedDelegate().AddUObject(this,&APlayerCharacter::OnPlayerControlSettingChanged);
	}

	GetGameInstance()->GetSubsystem<UMapSteamLoadSystem>()->SetCurrentCamera(this);
}

USpringArmComponent* APlayerCharacter::GetSpringArmComponent_Implementation()
{
	return SpringArmComp;
}

void APlayerCharacter::CallJump_Implementation()
{
	Jump();
}

void APlayerCharacter::OnPlayerControlSettingChanged(const FPlayerControlSettings& NewSettings)
{
	SpringArmComp->CameraLagSpeed = NewSettings.CameraDelaySpeed;
}

void APlayerCharacter::ActivateCamera(UPlayerBaseData* inPlayerData)
{
	if (OnInputChanged.IsBound())
	{
		OnInputChanged.Execute(true);
	}
}

void APlayerCharacter::DeactivateCamera(UPlayerBaseData* inPlayerData)
{
	if (OnInputChanged.IsBound())
	{
		OnInputChanged.Execute(false);
	}
}


