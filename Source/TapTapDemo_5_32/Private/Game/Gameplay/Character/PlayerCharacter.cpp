// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Character/PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Game/Gameplay/Component/CameraRegisterComponent.h"
#include "Game/Gameplay/Component/DialoageComponent.h"
#include "Game/Gameplay/Subsytem/GISubSystem/MapSteamLoadSystem.h"
#include "Game/Gameplay/Subsytem/GISubSystem/SoundSettingSubsystem.h"
#include "Game/Gameplay/Subsytem/GISubSystem/Data/MapSingleAsset.h"
#include "Game/GameplayTags/GameTags.h"
#include "Game/Gas/Attribute/AttributeSetBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"


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
	UGameInstance* GI =  GetGameInstance();
	
	GameSettingSubsys = GI->GetSubsystem<UGameSettingSubsystem>();
	if (ensure(GameSettingSubsys))
	{
		GameSettingSubsys->SetCameraDelaySpeed(SpringArmComp->CameraLagSpeed);
		GameSettingSubsys->GetPlayerControlChangedDelegate().AddUObject(this,&APlayerCharacter::OnPlayerControlSettingChanged);
		GameSettingSubsys->GetInputSensitivityDelegate().AddUObject(this,&APlayerCharacter::OnSensitivityChanged);
	}

	GI->GetSubsystem<UMapSteamLoadSystem>()->SetCurrentCamera(this);
	UMapSteamLoadSystem::GetOnMapChangedDelegate().AddDynamic(this,&APlayerCharacter::OnMapChanged);
}

USpringArmComponent* APlayerCharacter::GetSpringArmComponent_Implementation()
{
	return SpringArmComp;
}

void APlayerCharacter::CallJump_Implementation()
{
	Jump();
}

void APlayerCharacter::Die_Implementation()
{
	UKismetSystemLibrary::PrintString(GetWorld(),FString::Printf(TEXT("PlayerDead")));
}

void APlayerCharacter::BindAttributeDelegate()
{
	PlayerAbilitySysComp->GetGameplayAttributeValueChangeDelegate(UPlayerAttribute::GetMaxSpeedAttribute()).AddUObject(this,&APlayerCharacter::OnSpeedChanged);
	PlayerAbilitySysComp->GetGameplayAttributeValueChangeDelegate(UPlayerAttribute::GetHealthAttribute()).AddUObject(this,&APlayerCharacter::OnHealthChanged);
}

void APlayerCharacter::OnSpeedChanged(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

void APlayerCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue<=0)
	{
		ICharacterBaseInterface::Execute_Die(this);
	}
}

void APlayerCharacter::OnMapChanged(const FMapAssetData& Data)
{
	SetActorTransform(Data.MapStartTransform);
}

void APlayerCharacter::OnPlayerControlSettingChanged(const FPlayerControlSettings& NewSettings)
{
	SpringArmComp->CameraLagSpeed = NewSettings.CameraDelaySpeed;
	DialoageWidgetRoot->CameraLagSpeed = NewSettings.TextLag;
}

void APlayerCharacter::ActivateCamera(UPlayerBaseData* inPlayerData)
{
	if (GetInputChangedDelegate().IsBound())
	{
		GetInputChangedDelegate().Execute(true);
	}
}

void APlayerCharacter::DeactivateCamera(UPlayerBaseData* inPlayerData)
{
	if (GetInputChangedDelegate().IsBound())
	{
		GetInputChangedDelegate().Execute(false);
	}
}

void APlayerCharacter::OnSensitivityChanged(const FInputSensitivity& Newvalue)
{
	GetRotSensitivityChangedDelegate().Execute(Newvalue.RotatorSensitivity);
}



