// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Actor/SkillActor/SkillNodeActor.h"

#include "Components/WidgetComponent.h"
#include "Game/Gameplay/Component/NodeSplineComponent.h"
#include "Game/Gameplay/Interface/SkillNodeWidgetInterface.h"
#include "Game/Gameplay/Subsytem/Skill/SkillsManagerSubsystem.h"
#include "Game/Gameplay/Skills/SkillNode.h"
#include "Game/Gameplay/Subsytem/Skill/SkillGeneratorSubsystem.h"
#include "Game/GameplayTags/GameTags.h"

ASkillNodeActor::ASkillNodeActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene")));
	DetialWidgetComp = CreateDefaultSubobject<UWidgetComponent>("DetialWidget");
	DetialWidgetComp->SetupAttachment(GetRootComponent());
	
	NodeMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NodeMesh"));
	NodeMeshComp->SetupAttachment(GetRootComponent());
}

void ASkillNodeActor::BeginPlay()
{
	Super::BeginPlay();

	USkillsManagerSubsystem* SkillNodeManager = USkillsManagerSubsystem::Get(this);

	/*FSkillNodeInfo NodeInfo;
	
	LogicalNode = SkillNodeManager->NewSkillNode(NodeInfo);
	HashID = LogicalNode->GetHashID();*/

	Tags.Add(Game::Actor::Game_Actor_SkillNode.GetTag().GetTagName());

	//DetialWidget = Cast<ISkillNodeWidgetInterface>( DetialWidgetComp->GetWidget());
	//check(DetialWidget);
}

void ASkillNodeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//if (Data.NodeData){int32 Temp = Data.NodeData->Node->GetHashID();};
		
	if (isNeedUpdateSpline)
	{
		UpdateSpline();
	}
}

void ASkillNodeActor::IntialSkillNode(FNodeSaveData& Node)
{
	/***/
	/*//TODO::
	 * 初始化FConnectPackage 
	 * 并且初始化ConnectData;
	 */
	Data.NodeData = Node;
	Data.NodeHash = Data.NodeData.Node->GetHashID();;
	Data.MaxConnectCount = Data.NodeData.Node->GetNodeInfo().OutPinCount;
	Data.CanConnectHead = Data.NodeData.Node->GetNodeType()!=ESkillNodeType::StartNode;

	Intial();
}


/*void ASkillNodeActor::SetSpline(const struct FSplineConnectData& Data, uint32 Hash)
{
	 TailSplines.Emplace(Hash,FConnectPackage(Data));
	USkillsManagerSubsystem* SkillNodeManager = USkillsManagerSubsystem::Get(this);
	if (ensure(SkillNodeManager))
	{
		SkillNodeManager->ConnectNode(Data.HeadNode,Data.TailNode)
	}
}

void ASkillNodeActor::RemoveSpline(const uint32 ConnectPosHash)
{
	TailSplines.Remove(ConnectPosHash);
}*/

bool ASkillNodeActor::CanConnect(ESplineConnectType ConnectType)
{
	switch (ConnectType)
	{
	case ESplineConnectType::Head :
		{
			if (Data.CanConnectHead && (!HeadConnect.HaveConnect))
			{
				return true;
			};
			break;
		}
	case ESplineConnectType::Tail :
		{
			if (GetCurrentConnectCount()>0)
			{
				return true;
			}
			break;
		}
	}
	return false;
}

void ASkillNodeActor::OnlyRemoveSplineData(uint32 Hash)
{
	/**移除回调，在此之前已经调用移除了，现在是回调移除数据*/
	if (GetTypeHash(HeadConnect.SplineComp) == Hash) /**现在使用SlpineComp的指针作为Hash值来确定，每个Node不会有重复的SplineComponent*/
	{
		HeadConnect.SplineComp = nullptr;
		HeadConnect.HaveConnect = false;
		return;
	}
	auto FindComp = TailSplines.Find(Hash);
	if (FindComp)
	{
		TailSplines.Remove(Hash);
	}
}

const FVector ASkillNodeActor::GetHeadConnectPos()
{
	return NodeMeshComp->GetSocketLocation(TEXT("HeadConnect"));
}

const FVector ASkillNodeActor::GetTailConnectPos()
{
	return NodeMeshComp->GetSocketLocation(TEXT("TailConnect"));
}

bool ASkillNodeActor::CanPlay(const FVector& NewPos)
{
	if (!(false)/**check*/) //TODO::检测是否与其他node重叠,没有重叠就设置位置
	{
		return true;
	}
	return false;
}

void ASkillNodeActor::SetPos(const FVector& NewPos)
{
	SetActorLocation(NewPos);
}

void ASkillNodeActor::AddSplineRef(ESplineConnectType Type, UNodeSplineComponent* SplineComp)
{
	switch (Type)
	{
	case ESplineConnectType::Head :
		{
			HeadConnect.SplineComp = SplineComp;
			HeadConnect.HaveConnect = true;
			break;
		}
	case ESplineConnectType::Tail :
		{
			TailSplines.Emplace(GetTypeHash(SplineComp), FConnectPackage(SplineComp,true));
			break;
		}
	}
}

void ASkillNodeActor::StartMove()
{
	isNeedUpdateSpline = true;
	/*for (auto&i : TailSplines)
	{
		i.Value.SplineComp->HideSplineOnStartMove();
	}*/
}

void ASkillNodeActor::EndMove()
{
	isNeedUpdateSpline = false;
	/*for (auto&i : TailSplines)
	{
		i.Value.SplineComp->VisibleSplineOnEndMove();
		i.Value.SplineComp->UpdateSplineForConnect();
	}*/
}

void ASkillNodeActor::UpdateSpline()
{
	if ( HeadConnect.SplineComp)
	{
		HeadConnect.SplineComp->UpdateSplineForConnect();
	}
	for (auto&i : TailSplines)
	{
		i.Value.SplineComp->UpdateSplineForConnect();
	}
}


void ASkillNodeActor::RemoveNode()
{
	for (auto&i : TailSplines)
	{
		i.Value.SplineComp->DeactiveSpline();
	}
	Destroy();
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

void ASkillNodeActor::SelectNode()
{
	SetSelectState(true);
}

void ASkillNodeActor::UnSelectNode()
{
	SetSelectState(false);
}

void ASkillNodeActor::StartHolding()
{
	StartMove();
}

void ASkillNodeActor::EndHolding()
{
	EndMove();
}



void ASkillNodeActor::BreakHead()
{
	if (HeadConnect.SplineComp&&HeadConnect.HaveConnect)
	{
		HeadConnect.SplineComp->DeactiveSpline();
	}
}

void ASkillNodeActor::ConnectFaildTips()
{
	UE_LOG(LogTemp,Error,TEXT("ASkillNodeActor::ConnectFaildTips :: Actor = %s"),*this->GetName());
}

int32 ASkillNodeActor::GetCurrentConnectCount()
{
	return Data.MaxConnectCount-TailSplines.Num();
}

