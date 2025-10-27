// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Actor/SkillActor/SkillNodeActor.h"

#include "Components/WidgetComponent.h"
#include "Game/Gameplay/Component/NodeSplineComponent.h"
#include "Game/Gameplay/Interface/SkillNodeWidgetInterface.h"
#include "Game/Gameplay/Subsytem/Skill/SkillsManagerSubsystem.h"
#include "Game/Gameplay/Skills/SkillNode.h"
#include "Game/GameplayTags/GameTags.h"

ASkillNodeActor::ASkillNodeActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene")));
	DetialWidgetComp = CreateDefaultSubobject<UWidgetComponent>("DetialWidget");
	DetialWidgetComp->SetupAttachment(GetRootComponent());
}

void ASkillNodeActor::BeginPlay()
{
	Super::BeginPlay();

	USkillsManagerSubsystem* SkillNodeManager = USkillsManagerSubsystem::Get(this);

	FSkillNodeInfo NodeInfo;
	
	LogicalNode = SkillNodeManager->NewSkillNode(NodeInfo);
	HashID = LogicalNode->GetHashID();

	Tags.Add(Game::Actor::Game_Actor_SkillNode.GetTag().GetTagName());

	//DetialWidget = Cast<ISkillNodeWidgetInterface>( DetialWidgetComp->GetWidget());
	//check(DetialWidget);
}

void ASkillNodeActor::SetSpline(const struct FSplineConnectData& Data, uint32 Hash)
{
	 Splines.Emplace(Hash,Data);
}

void ASkillNodeActor::RemoveSpline(const uint32 ConnectPosHash)
{
	Splines.Remove(ConnectPosHash);
}

void ASkillNodeActor::UpdateSpline()
{
	for (auto&i : Splines)
	{
		i.Value.NodeSplineComp->UpdateSplineForConnect();
	}
}

void ASkillNodeActor::StartTouching()
{
	ISkillNodeWidgetInterface::Execute_ActivateDetial(DetialWidgetComp->GetWidget());
	MouseTouch(true);
}

void ASkillNodeActor::EndTouching()
{
	ISkillNodeWidgetInterface::Execute_DeactivateDetial(DetialWidgetComp->GetWidget());
	MouseTouch(false);
}

void ASkillNodeActor::StartHolding()
{
	
}

void ASkillNodeActor::EndHolding()
{
	
}

