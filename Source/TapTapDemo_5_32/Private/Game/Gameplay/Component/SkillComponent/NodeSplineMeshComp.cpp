// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Component/SkillComponent/NodeSplineMeshComp.h"

UNodeSplineMeshComp::UNodeSplineMeshComp()
{
	Mobility = EComponentMobility::Type::Movable;
}

void UNodeSplineMeshComp::SetPoolState(bool isVis)
{
	SetVisibility(isVis);
	UE_LOG(LogTemp,Display,TEXT("SetPoolState::%s::Vis = %s"),*GetName(),*LexToString(isVis));
	if (isVis&&(!IsRegistered()))
	{
		RegisterComponent();
	}
}


