// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Component/NodeSplineComponent.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Game/Gameplay/Interface/NodeSplineInterface.h"
#include "Kismet/KismetSystemLibrary.h"


UNodeSplineComponent::UNodeSplineComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNodeSplineComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UNodeSplineComponent::UpdateSplineForConnect()
{
	UNavigationPath* Result = UNavigationSystemV1::FindPathToLocationSynchronously(
	GetWorld(),
	ConnectNodeData[0].ConnectActor->GetActorLocation()+ConnectNodeData[0].ConnectOffset,
	ConnectNodeData[1].ConnectActor->GetActorLocation()+ConnectNodeData[1].ConnectOffset
	);
	if (ensure(Result))
	{
		const TArray<FVector>Path = Result->PathPoints;
		SetSplinePoints(Path,ESplineCoordinateSpace::Local);
		for (auto& i:Result->PathPoints)
		{
			UKismetSystemLibrary::DrawDebugSphere(GetWorld(),i+FVector(0,0,50),10,12,FLinearColor::Red,100.,10.);
		}
	}
}

void UNodeSplineComponent::ActiveSpline(const FNodeConnectData& Head, const FNodeConnectData& Tail)
{
	ConnectNodeData[0] = Head;
	ConnectNodeData[1] = Tail;
	UpdateSplineForConnect();
}

void UNodeSplineComponent::DeactiveSpline()
{
	if (ensure( OnDeactive.IsBound()))
	{
		OnDeactive.Execute(this);
		HideSpline();
		INodeSplineInterface* HeadNodeInterface = Cast<INodeSplineInterface>(ConnectNodeData[0].ConnectActor);
		INodeSplineInterface* TailNodeInterface = Cast<INodeSplineInterface>(ConnectNodeData[1].ConnectActor);
		if ((!HeadNodeInterface) || (!TailNodeInterface))
		{
			UE_LOG(LogTemp, Error, TEXT("UNodeSplineComponent::DeactiveSpline::InterfaceCastFaild"));
			check(0);
			return;
		}
		HeadNodeInterface->RemoveSpline(GetTypeHash(ConnectNodeData[0].ConnectOffset));
		TailNodeInterface->RemoveSpline(GetTypeHash(ConnectNodeData[1].ConnectOffset));

		ConnectNodeData[0].ConnectActor =nullptr;
		ConnectNodeData[1].ConnectActor =nullptr;
	}
}

void UNodeSplineComponent::HideSpline()
{
	//TODO::
	UE_LOG(LogTemp, Warning, TEXT("HideSpline::Nothing::NeedTodo"));
	SetVisibility(false);
}

void UNodeSplineComponent::ShowSpline()
{
	//TODO::
	UE_LOG(LogTemp, Warning, TEXT("HideSpline::Nothing::NeedTodo"));
	SetVisibility(true);
}

