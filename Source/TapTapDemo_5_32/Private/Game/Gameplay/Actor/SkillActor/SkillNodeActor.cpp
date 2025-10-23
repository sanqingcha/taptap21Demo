// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Actor/SkillActor/SkillNodeActor.h"

#include "Game/Gameplay/Component/NodeSplineComponent.h"
#include "Game/Gameplay/Subsytem/Skill/SkillsManagerSubsystem.h"
#include "Game/Gameplay/Skills/SkillNode.h"
#include "Game/Gameplay/Subsytem/Skill/SplineSpawnSystem.h"

ASkillNodeActor::ASkillNodeActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASkillNodeActor::BeginPlay()
{
	Super::BeginPlay();

	USkillsManagerSubsystem* SkillNodeManager = USkillsManagerSubsystem::Get(this);

	FSkillNodeInfo NodeInfo;
	
	LogicalNode = SkillNodeManager->NewSkillNode(NodeInfo);
	HashID = LogicalNode->GetHashID();
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

