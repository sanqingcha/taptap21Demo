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
#include "Game/Gameplay/Subsytem/GISubSystem/MapSteamLoadSystem.h"
#include "Game/Gameplay/Subsytem/Skill/SkillsRuntimeSubsystem.h"

UPlayerBaseData* ASuperPlayerController::PlayerData_Static = nullptr;

ASuperPlayerController::ASuperPlayerController()
{
	
}

void ASuperPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	CheckResource();
	InitialHUD();
	InitialPawn();
	intialControl();

	SkillRuntimeSys = GetGameInstance()->GetSubsystem<USkillsRuntimeSubsystem>();
	check(SkillRuntimeSys);
	
}

void ASuperPlayerController::intialControl()
{
	check(PlayerData)
	/*
	 *UAbilitySystemComponent* ASC = PlayerData->ASC;
	 *FGameplayTagContainer BlockInputGameplayTags;
	 *BlockInputGameplayTags.AddTag(Game::Input::Game_Input_AttackBlock);
	 *BlockInputGameplayTags.AddTag(Game::Input::Game_Input_JumpBlock);
	 *BlockInputGameplayTags.AddTag(Game::Input::Game_Input_MoveBlock);
	 *BlockInputGameplayTags.AddTag(Game::Input::Game_Input_RotateBlock);
	 *ASC->AddLooseGameplayTags(BlockInputGameplayTags);
	 *@Param :: MappingContext :: 直接使用Mapping的注册来控制玩家输入
	 */
	UMapSteamLoadSystem::GetOnStreamLoadOverDelegate().AddDynamic(this,&ASuperPlayerController::OnMapOpen);
}

void ASuperPlayerController::OnMapOpen()
{
	UMapSteamLoadSystem::GetOnStreamLoadOverDelegate().RemoveDynamic(this,&ASuperPlayerController::OnMapOpen);
	SetShowMouseCursor(false);
	check(PlayerData)
	PlayerData->EnhancedInputSys->AddMappingContext(GameInputContext, 0);
	
	/*UAbilitySystemComponent* ASC = PlayerData->ASC;
	 *FGameplayTagContainer BlockInputGameplayTags;
	 *BlockInputGameplayTags.AddTag(Game::Input::Game_Input_AttackBlock);
	 *BlockInputGameplayTags.AddTag(Game::Input::Game_Input_JumpBlock);
	 *BlockInputGameplayTags.AddTag(Game::Input::Game_Input_MoveBlock);
	 *BlockInputGameplayTags.AddTag(Game::Input::Game_Input_RotateBlock);
	 *ASC->RemoveLooseGameplayTags(BlockInputGameplayTags);
	 *@Param :: MappingContext :: 直接使用Mapping的注册来控制玩家输入
	 */

}


void ASuperPlayerController::CheckResource()
{
	check(MoveAction);
	check(RotateAction);
	check(JumpAction);
	check(SkillActionsData);
}

void ASuperPlayerController::InitialPawn()
{
	SetControlRotation(initRotator);
	PlayerPawnInterface = Cast<IPlayerPawnInterface> (GetPawn());
	check(PlayerPawnInterface);
	PlayerPawnInterface->GetInputChangedDelegate().BindUObject(this,&ASuperPlayerController::OnPlayerInputMappingChanged);
	PlayerPawnInterface->GetRotSensitivityChangedDelegate().BindUObject(this,&ASuperPlayerController::OnRotSensitivityChanged);
}

void ASuperPlayerController::OnPlayerInputMappingChanged(bool Activate)
{
	if (Activate)
	{
		PlayerData->EnhancedInputSys->AddMappingContext(GameInputContext, 0);
	}
	else
	{
		PlayerData->EnhancedInputSys->RemoveMappingContext(GameInputContext);
	}
}

void ASuperPlayerController::InitialHUD()
{
	IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(GetPawn());
	if (!ensure(AbilityInterface)){return;}
	AbilitySysComp = AbilityInterface->GetAbilitySystemComponent();
	IHUDInterface* HUDInterface = Cast<IHUDInterface>(GetHUD());
	UEnhancedInputLocalPlayerSubsystem* EnhanceSubSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent); 
	check(HUDInterface);
	PlayerData = NewObject<UPlayerBaseData>(this);
	PlayerData->SetData(this,GetPlayerState<APlayerState>(),AbilitySysComp,EnhanceSubSys,EnhancedInputComp);
	PlayerData_Static = PlayerData;
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
		EnhancedInputComp->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASuperPlayerController::Jump_Action);
		
		if (SkillActionsData!=nullptr)
		{
			for (auto& i : SkillActionsData->SkillInputData)
			{
				EnhancedInputComp->BindAction(i.SkillInputAction, ETriggerEvent::Started, this, &ThisClass::TryUseSkill,i.Type);
				//TODO::初始化技能头模块数量以及映射到技能模块,使用Tag标记每个创建开始模块;
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

	const FVector2D InputValue = Value.Get<FVector2D>()*RotMut;
	AddYawInput(InputValue.X);
	AddPitchInput(InputValue.Y);
}

void ASuperPlayerController::Jump_Action(const FInputActionValue& Value)
{
	if (AbilitySysComp&&AbilitySysComp->HasMatchingGameplayTag(Game::Input::Game_Input_MoveBlock)){return;}
	if (AbilitySysComp&&AbilitySysComp->HasMatchingGameplayTag(Game::Input::Game_Input_JumpBlock)){return;}
	IPlayerPawnInterface::Execute_CallJump(GetPawn());
}



void ASuperPlayerController::TryUseSkill(EStartNodeType Type)
{
	if (AbilitySysComp&&AbilitySysComp->HasMatchingGameplayTag(Game::Input::Game_Input_MoveBlock)){return;}
	if (AbilitySysComp&&AbilitySysComp->HasMatchingGameplayTag(Game::Input::Game_Input_AttackBlock)){return;}

	SkillRuntimeSys->TriggerStartNode(Type);
}

