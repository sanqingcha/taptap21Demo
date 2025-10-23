// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Player/SuperPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Game/Gameplay/Player/Data/PlayerBaseData.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerState.h"
#include "Game/Gameplay/Interface/HUDInterface.h"
#include "Game/Gameplay/Interface/PlayerPawnInterface.h"
#include "Game/GameplayTags/GameTags.h"
#include "Game/Gas/Attribute/AttributeSetBase.h"
#include "Game/Gameplay/Player/Data/SkillInputAsset.h"


ASuperPlayerController::ASuperPlayerController()
{
}

void ASuperPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CheckResource();
	InitialHUD();
	InitialPawn();
}


void ASuperPlayerController::CheckResource()
{
	check(MoveAction);
	check(RotateAction);
	check(JumpAction);
	check(ShootAction);
}

void ASuperPlayerController::InitialPawn()
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(GameInputContext, 0);
	}
	PlayerPawnInterface = Cast<IPlayerPawnInterface> (GetPawn());
	check(PlayerPawnInterface);
}

void ASuperPlayerController::InitialHUD()
{
	IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(GetPawn());
	if (!ensure(AbilityInterface)){return;}
	AbilitySysComp = AbilityInterface->GetAbilitySystemComponent();
	IHUDInterface* HUDInterface = Cast<IHUDInterface>(GetHUD());
	check(HUDInterface);
	PlayerData = NewObject<UPlayerBaseData>();
	PlayerData->SetData(this,GetPlayerState<APlayerState>(),AbilitySysComp);
	HUDInterface->InitialHUD(PlayerData);
}

void ASuperPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent); 
	if (ensure(EnhancedInputComp))
	{
		EnhancedInputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASuperPlayerController::Move_Action);
		EnhancedInputComp->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ASuperPlayerController::Rotate_Action);
		EnhancedInputComp->BindAction(JumpAction, ETriggerEvent::Started, this, &ASuperPlayerController::Jump_Action);
		EnhancedInputComp->BindAction(ShootAction,ETriggerEvent::Started,this,&ASuperPlayerController::Shoot_Action);
		
		if (SkillActionsData!=nullptr)
		{
			for (auto& i : SkillActionsData->SkillInputData)
			{
				EnhancedInputComp->BindAction(i.Value.SkillInputAction, i.Value.TrriggerType, this, &ThisClass::TryUseSkill,i.Key);
				//TODO::初始化技能头模块数量以及映射到技能模块
			}
		}
		
	}
}

void ASuperPlayerController::Move_Action(const FInputActionValue& Value)
{
	if (AbilitySysComp&&AbilitySysComp->HasMatchingGameplayTag(Game::Input::Game_Input_MoveBlock)){return;}
	const FVector Forward = FVector(GetPawn()->GetActorForwardVector().X,GetPawn()->GetActorForwardVector().Y,0.);
	const FVector Right = FVector(GetPawn()->GetActorRightVector().X,GetPawn()->GetActorRightVector().Y,0.); 
	const FVector2D InputValue = Value.Get<FVector2D>();
	GetPawn()->AddMovementInput(Forward,InputValue.Y);
	GetPawn()->AddMovementInput(Right,InputValue.X);
}

void ASuperPlayerController::Rotate_Action(const FInputActionValue& Value)
{
	if (AbilitySysComp&&AbilitySysComp->HasMatchingGameplayTag(Game::Input::Game_Input_RotateBlock)){return;}

	const FVector2D InputValue = Value.Get<FVector2D>();
	AddYawInput(InputValue.X);
	AddPitchInput(InputValue.Y);
}

void ASuperPlayerController::Jump_Action(const FInputActionValue& Value)
{
	if (AbilitySysComp&&AbilitySysComp->HasMatchingGameplayTag(Game::Input::Game_Input_MoveBlock)){return;}
	if (AbilitySysComp&&AbilitySysComp->HasMatchingGameplayTag(Game::Input::Game_Input_JumpBlock)){return;}
	IPlayerPawnInterface::Execute_CallJump(GetPawn());
}

void ASuperPlayerController::Shoot_Action(const FInputActionValue& Value)
{
	if (AbilitySysComp&&AbilitySysComp->HasMatchingGameplayTag(Game::Input::Game_Input_AttackBlock)){return;}
	UE_LOG(LogTemp,Warning,TEXT("Shooting"));
}

void ASuperPlayerController::TryUseSkill(FGameplayTag Tag)
{
	//TODO::尝试释放对应模块技能
	unimplemented();
	UE_LOG(LogTemp,Warning,TEXT("TryUseSkill :: Tag = %s"), * Tag.ToString());
}
