// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Character/PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Game/Gameplay/Component/DialoageComponent.h"
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
	
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayerAbilitySysComp->InitAbilityActorInfo(this,this);
}

USpringArmComponent* APlayerCharacter::GetSpringArmComponent_Implementation()
{
	return SpringArmComp;
}

void APlayerCharacter::CallJump_Implementation()
{
	Jump();
}


