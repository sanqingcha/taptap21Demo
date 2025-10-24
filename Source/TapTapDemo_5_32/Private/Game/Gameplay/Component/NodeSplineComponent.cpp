// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Component/NodeSplineComponent.h"
#include "Game/Gameplay/Component/SkillComponent/NodeSplineMeshComp.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Game/Gameplay/Interface/NodeSplineInterface.h"
#include "Game/Gameplay/Subsytem/Manager/PoolManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Virtualization/VirtualizationTypes.h"


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
	check(SplineStaticMesh);
	
	UNavigationPath* Result = UNavigationSystemV1::FindPathToLocationSynchronously(
	GetWorld(),
	ConnectNodeData[0].ConnectActor->GetActorLocation()+ConnectNodeData[0].ConnectOffset,
	ConnectNodeData[1].ConnectActor->GetActorLocation()+ConnectNodeData[1].ConnectOffset
	);
	if (ensure(Result))
	{
		const TArray<FVector>Path = Result->PathPoints;
		SetSplinePoints(Path,ESplineCoordinateSpace::Local);
		/*for (auto& i:Path)
		{
			UKismetSystemLibrary::DrawDebugSphere(GetWorld(),
				i+FVector(0,0,50),
				10,
				12,
				FLinearColor::Red,
				GetWorld()->GetDeltaSeconds()*5,
				10.);
		}*/
		AActor* SplineOwner = GetOwner();
		UPoolManager* PoolManager = GetWorld()->GetSubsystem<UPoolManager>();
		if (PoolManager)
		{
			for (int32 i  = 0; i<= Path.Num()-2; i++)
			{
				//UE_LOG(LogTemp,Error,TEXT("PathPointNum = %d"),Path.Num()-1)
				UNodeSplineMeshComp* NewMeshComp = nullptr; 
				if (SplinMeshs.IsValidIndex(i))
				{
					NewMeshComp = SplinMeshs[i];
					//UE_LOG(LogTemp,Error,TEXT("SaveNum = %d"),SplinMeshs.Num())
				}
				else
				{
					//UE_LOG(LogTemp,Error,TEXT("NewNum = %d"),SplinMeshs.Num()-(Path.Num()-1))
					PoolManager->GET_OR_CREATE_PTR(NewMeshComp,SplineOwner,UNodeSplineMeshComp);
					SplinMeshs.Add(NewMeshComp);
					NewMeshComp->SetStaticMesh(SplineStaticMesh);
				}
				FVector StartPos;;
				FVector StartTan;
				GetLocationAndTangentAtSplinePoint(i,StartPos,StartTan,ESplineCoordinateSpace::Local);
				FVector EndPos;
				FVector EndTan;
				GetLocationAndTangentAtSplinePoint(i+1,EndPos,EndTan,ESplineCoordinateSpace::Local);
				NewMeshComp->SetStartAndEnd(
					StartPos,
					StartTan,
					EndPos,
					EndTan
					);
			}
			if (SplinMeshs.Num()>Path.Num()-1&&Path.IsValidIndex(0))
			{
				for (int32 i = Path.Num()-1;i<SplinMeshs.Num();i++)
				{
					PoolManager->DEACTIVATE_PTR(SplinMeshs[i],UNodeSplineMeshComp);
				}
				SplinMeshs.RemoveAt(Path.Num()-1,SplinMeshs.Num()-(Path.Num()-1));
			}
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

