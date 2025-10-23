// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Actor/SkillActor/SplineVisualizeActor.h"

#include "Game/Gameplay/Component/NodeSplineComponent.h"
#include "Game/Gameplay/Subsytem/Skill/SplineSpawnSystem.h"


// Sets default values
ASplineVisualizeActor::ASplineVisualizeActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASplineVisualizeActor::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetSubsystem<USplineSpawnSystem>()->RegisterSplineActor(this);
}

UNodeSplineComponent* ASplineVisualizeActor::GetOrCreateSplineComponent()
{
	UNodeSplineComponent* NewSplineComp = nullptr;
	if (SplinePool.IsEmpty())
	{
		NewSplineComp = NewObject<UNodeSplineComponent>(this);
		NewSplineComp->OnDeactive.BindUObject(this,&ASplineVisualizeActor::RemoveSplineComponent);
	}
	else
	{
		SplinePool.Dequeue(NewSplineComp);
	}
	return NewSplineComp;
}

void ASplineVisualizeActor::RemoveSplineComponent(UNodeSplineComponent* TargetNode)
{
	SplinePool.Enqueue(TargetNode);
}


