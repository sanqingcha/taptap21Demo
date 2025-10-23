// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Subsytem/Skill/SplineSpawnSystem.h"

#include "Game/Gameplay/Actor/SkillActor/SplineVisualizeActor.h"
#include "Game/Gameplay/Component/NodeSplineComponent.h"
#include "Game/Gameplay/Interface/NodeSplineInterface.h"

void USplineSpawnSystem::RegisterSplineActor(ASplineVisualizeActor* inSplineVisActor)
{
	SplineVisActor = inSplineVisActor;
}

void USplineSpawnSystem::SpawnSpline(const FNodeConnectData& ConnectHead, const FNodeConnectData& ConnectTail)
{
	check(SplineVisActor);

	INodeSplineInterface* HeadNodeInterface = Cast<INodeSplineInterface>(ConnectHead.ConnectActor);
	INodeSplineInterface* TailNodeInterface = Cast<INodeSplineInterface>(ConnectTail.ConnectActor);
	if ((!HeadNodeInterface) || (!TailNodeInterface))
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor没有实现接口，需要实现INodeSplineInterface"));
		return;
	}
	UNodeSplineComponent* NodeConnectSplineComp = SplineVisActor->GetOrCreateSplineComponent();
	NodeConnectSplineComp->ActiveSpline(ConnectHead, ConnectTail);
	/**使用链接点相对SkillNodeActor位置的偏移量作为Hash值记录对应的SplineComponent*/
	HeadNodeInterface->SetSpline(FSplineConnectData(NodeConnectSplineComp,ESplineConnectType::Head),GetTypeHash(ConnectHead.ConnectOffset));
	TailNodeInterface->SetSpline(FSplineConnectData(NodeConnectSplineComp,ESplineConnectType::Tail),GetTypeHash(ConnectHead.ConnectOffset));
}


