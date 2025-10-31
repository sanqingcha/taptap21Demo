// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Component/NodeSplineComponent.h"
#include "Game/Gameplay/Component/SkillComponent/NodeSplineMeshComp.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Game/Gameplay/Interface/SkillNodeInteractInterface.h"
#include "Game/Gameplay/Subsytem/Manager/PoolManager.h"
#include "Game/Gameplay/Subsytem/Skill/SkillsManagerSubsystem.h"


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
	
	/*UNavigationPath* Result = UNavigationSystemV1::FindPathToLocationSynchronously(
	GetWorld(),
	ConnectNodeData[0]->GetTailConnectPos(),
	ConnectNodeData[1]->GetHeadConnectPos()
	);*/

	/*UE_LOG(LogTemp,Error,TEXT("ConnectPosHeadHead = %s,ConnectPosTail = %s"),
		*(ConnectNodeData[0]->GetTailConnectPos().ToString()),
		*(ConnectNodeData[1]->GetHeadConnectPos().ToString())
		);*/
	
	if (/***ensure(Result)||*/true)
	{
		
		//const TArray<FVector>Path = Result->PathPoints;
		const TArray<FVector> Path  = {	ConnectNodeData[0]->GetTailConnectPos(),
	ConnectNodeData[1]->GetHeadConnectPos()};
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
					
				}
				NewMeshComp->SetStaticMesh(SplineStaticMesh);
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

void UNodeSplineComponent::ActiveSpline(ISkillNodeInteractInterface* Head, ISkillNodeInteractInterface* Tail)
{
	ConnectNodeData[0] = Head;
	ConnectNodeData[1] = Tail;
	
	/**技能逻辑已被链接并且判断可以连接了，这里直接把可视化线条的部分链接上*/
	
	/***/
	ShowSpline();
	UpdateSplineForConnect();
}

void UNodeSplineComponent::DeactiveSpline()
{
	if (ensure( OnDeactive.IsBound()))
	{
		OnDeactive.Execute(this); /**通知对象回收对象*/

		if (ConnectNodeData[0]&&ConnectNodeData[1])
		{
			USkillsManagerSubsystem* Skillmanager = USkillsManagerSubsystem::Get(GetWorld());
			Skillmanager->DisconnectNode(ConnectNodeData[0]->GetSkillNode(),ConnectNodeData[1]->GetSkillNode());
			ConnectNodeData[0]->OnlyRemoveSplineData(GetTypeHash(this));
			ConnectNodeData[1]->OnlyRemoveSplineData(GetTypeHash(this));
		}
		HideSpline();
	}
}

void UNodeSplineComponent::HideSplineOnStartMove()
{
	HideSpline();
}

void UNodeSplineComponent::VisibleSplineOnEndMove()
{
	ShowSpline();
}

void UNodeSplineComponent::HideSpline()
{
	//TODO::
	//UE_LOG(LogTemp, Warning, TEXT("HideSpline::Nothing::NeedTodo"));
	SetVisibility(false);
	for (auto& i: SplinMeshs)
	{
		i->SetVisibility(false);
	}
}

void UNodeSplineComponent::ShowSpline()
{
	//TODO::
	//UE_LOG(LogTemp, Warning, TEXT("HideSpline::Nothing::NeedTodo"));
	SetVisibility(true);
	for (auto& i: SplinMeshs)
	{
		i->SetVisibility(true);
	}
}

