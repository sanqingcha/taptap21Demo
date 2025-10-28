// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Subsytem/Skill/SplineSpawnSystem.h"

#include "Game/Gameplay/Actor/SkillActor/SplineVisualizeActor.h"
#include "Game/Gameplay/Component/NodeSplineComponent.h"
#include "Game/Gameplay/Interface/NodeSplineInterface.h"
#include "Game/Gameplay/Interface/SkillNodeInteractInterface.h"
#include "Game/Gameplay/Subsytem/Skill/SkillsManagerSubsystem.h"


void USplineSpawnSystem::RegisterSplineActor(ASplineVisualizeActor* inSplineVisActor)
{
	SplineVisActor = inSplineVisActor;
}

 UNodeSplineComponent*USplineSpawnSystem::TryConnect(ISkillNodeInteractInterface* Head,ISkillNodeInteractInterface* Tail,bool& Success)
{
	check(SplineVisActor);
	if (!ensure(Head) || !ensure(Tail)) return nullptr;
	
	if (Head->CanConnect(ESplineConnectType::Tail)&& 
		Tail->CanConnect(ESplineConnectType::Head))
	{
		USkillNode* HeadSkill = Head->GetSkillNode();
		USkillNode* TailSkill = Tail->GetSkillNode();

		USkillsManagerSubsystem* Skillmanager = USkillsManagerSubsystem::Get(GetWorld());
		
		Success = true;//Skillmanager->ConnectNode(HeadSkill,TailSkill);

		if (Success==false)
		{
			return nullptr;
		}
		
		UNodeSplineComponent* NodeConnectSplineComp = SplineVisActor->GetOrCreateSplineComponent();
		NodeConnectSplineComp->ActiveSpline(Head, Tail);

		Head->AddSplineRef(ESplineConnectType::Tail, NodeConnectSplineComp);
		Tail->AddSplineRef(ESplineConnectType::Head, NodeConnectSplineComp);
		
		Success = true;
		return NodeConnectSplineComp;
	};
	Success = false;
	return nullptr;
}



