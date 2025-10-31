// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Subsytem/Skill/SkillsManagerSubsystem.h"

#include "Game/Gameplay/Skills/SkillNode.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

#include <random>

#include "Game/Gameplay/Subsytem/Skill/SkillsRuntimeSubsystem.h"

const float USkillsManagerSubsystem::AdditionalParamForAttack = 10.0f;

void USkillsManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	std::random_device _seed;
	HashGenerateEngine.seed(_seed());
}

USkillNode* USkillsManagerSubsystem::NewSkillNode(const FSkillNodeInfo& NodeInfo, TSubclassOf<USkillNode> SkillNodeClass)
{
	std::uniform_int_distribution<> distribution(0, 0x3fffffff);
	int32 HashID = distribution(HashGenerateEngine);
    
	USkillNode* Node = NewObject<USkillNode>(this, SkillNodeClass);
	Node->Initialize(HashID, NodeInfo);
    
	HashToNode.Add(HashID, Node);
    
	if (NodeInfo.NodeType == ESkillNodeType::StartNode)
	{
		GetSkillsRuntimeSubsystem()->AddStartSkillNode(Node);
	}
    	
	return Node;
}

void USkillsManagerSubsystem::DeleteSkillNode(USkillNode* Node, OnBranchNode Branch)
{
	HashToNode.Remove(Node->GetHashID());

	USkillNode* TempChildNode = Node->GetFirstChildNode();
	
	Node->ForceRemoveAllChild();

	FlushParentLoopStartNodeWithSelf(Node);

	if (Node->GetNodeType() == ESkillNodeType::LoopEndNode)
	{
		ConnectNode(Node->GetParentNode(), TempChildNode);
	}
	
	if (Node->GetParentNode())
		DisconnectNode(Node->GetParentNode(), Node, Branch);

	if (Node->GetNodeType() == ESkillNodeType::StartNode)
		GetSkillsRuntimeSubsystem()->RemoveStartSkillNode(Node);
	
	Node->MarkAsGarbage();
}

bool USkillsManagerSubsystem::ConnectNode(USkillNode* ParentNode, USkillNode* ChildNode, OnBranchNode Branch)
{
	if (!ParentNode || !ChildNode) return false;
	if (ParentNode->GetHashID() == ChildNode->GetHashID()) return false;
	
	if (!ParentNode->AddChildNode(ChildNode, Branch)) return false;
	
	ChildNode->SetParentNode(ParentNode);
	FlushParentLoopStartNodeWithSelf(ChildNode);
	if (ParentNode->GetNodeType() == ESkillNodeType::ParamNode)
		DfsUpdateForwardParam(ChildNode);
	else
		UpdateTimeDelay(ParentNode);
	return true;
}

void USkillsManagerSubsystem::DisconnectNode(USkillNode* ParentNode, USkillNode* ChildNode, OnBranchNode Branch)
{
	if (!ParentNode || !ChildNode) return;
	if (ChildNode->GetParentNode()->GetHashID() != ParentNode->GetHashID()) return;
	
	ParentNode->RemoveChildNode(ChildNode, Branch);
	FlushParentLoopStartNode(ChildNode);
	UpdateTimeDelay(ParentNode);
	ChildNode->SetParentNode(nullptr);
	ChildNode->SetAccumulativeInfo(FAccumulativeInfo::ZeroValue);
	DfsUpdateForwardParam(ChildNode);
}

void USkillsManagerSubsystem::UpdateLoopEndNodes(TArray<USkillNode*>& LoopEndArray, USkillNode* LoopStartNode)
{
	for (auto& Node : LoopEndArray)
	{
		USkillNode* ParentNode = Node->GetParentNode();
		USkillNode* ChildNode = Node->GetFirstChildNode();

		if (!ParentNode)
		{
			DeleteSkillNode(Node);
			continue;
		}
		
		// 如果是分支类型
		if (ParentNode->GetNodeType() == ESkillNodeType::BranchNode)
		{
			if (ParentNode->GetBranchTrueNode() && ParentNode->GetBranchTrueNode()->GetHashID() == Node->GetHashID())
			{
				DeleteSkillNode(Node, OnBranchNode::TrU);
				if (ChildNode) ConnectNode(ParentNode, ChildNode, OnBranchNode::TrU);
			}

			if (ParentNode->GetBranchFalseNode() && ParentNode->GetBranchFalseNode()->GetHashID() == Node->GetHashID())
			{
				DeleteSkillNode(Node, OnBranchNode::FaL);
				if (ChildNode) ConnectNode(ParentNode, ChildNode, OnBranchNode::FaL);
			}

			continue;
		}

		DeleteSkillNode(Node);
		if (ChildNode) ConnectNode(ParentNode, ChildNode);
	}
	LoopEndArray.Empty();

	TemporaryLoopEndNodeArray = &LoopEndArray;
	TemporaryLoopStartNodeBehindTarget.Empty();
	DfsStuffEndNodes(LoopStartNode);
	LoopEndArray = *TemporaryLoopEndNodeArray;
}

USkillNode* USkillsManagerSubsystem::GetSkillNodeByHash(const int32 _HashID) const
{
	return HashToNode.Contains(_HashID) ? HashToNode[_HashID] : nullptr;
}

USkillsManagerSubsystem* USkillsManagerSubsystem::Get(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return nullptr;
	UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
	return GameInstance ? GameInstance->GetSubsystem<USkillsManagerSubsystem>() : nullptr;
}

void USkillsManagerSubsystem::SetCanRun(bool Value)
{
	bCanRun = Value;
}

bool USkillsManagerSubsystem::GetCanRun()
{
	return bCanRun;
}

void USkillsManagerSubsystem::DfsStuffEndNodes(USkillNode* CurrentNode)
{
	if (!CurrentNode) return;

	// 找到循环起点就加入列表
	if (CurrentNode->GetNodeType() == ESkillNodeType::LoopStartNode)
	{
		TemporaryLoopStartNodeBehindTarget.Add(CurrentNode->GetHashID());
	}

	// 遇到循环终点
	if (CurrentNode->GetNodeType() == ESkillNodeType::LoopEndNode)
	{
		// 如果遇到没有经过起点的终点，也就是就说找到了放置位置，等同于遍历到叶节点，新节点会被插入到这个End之前
		int32 LoopStartNodeHashID = CurrentNode->GetLoopStartNode()->GetHashID();
		if (!TemporaryLoopStartNodeBehindTarget.Contains(LoopStartNodeHashID))
		{
			FSkillNodeInfo NodeInfo;
			NodeInfo.NodeType = ESkillNodeType::LoopEndNode;
			NodeInfo.DelayTime = 0.0f;
			USkillNode* NewNode = NewSkillNode(NodeInfo, USkillNode::StaticClass());

			// 当前应该形成的结构：ParentNode->NewNode->CurrentNode

			// 重置 ParentNode 和 NewNode 的双向连接
			USkillNode* ParentNode = CurrentNode->GetParentNode();
			ParentNode->RemoveChildNode(CurrentNode);
			if (ParentNode->GetNodeType() == ESkillNodeType::BranchNode)
			{
				if (ParentNode->GetBranchTrueNode() && ParentNode->GetBranchTrueNode()->GetHashID() == CurrentNode->GetHashID())
				{
					ConnectNode(ParentNode, NewNode, OnBranchNode::TrU);
				}

				if (ParentNode->GetBranchFalseNode() && ParentNode->GetBranchFalseNode()->GetHashID() == CurrentNode->GetHashID())
				{
					ConnectNode(ParentNode, NewNode, OnBranchNode::FaL);
				}
			}
			else
				ConnectNode(CurrentNode->GetParentNode(), NewNode);

			// 双向连接 NewNode 与 CurrentNode
			CurrentNode->SetParentNode(NewNode);
			NewNode->AddChildNode(CurrentNode);
			
			TemporaryLoopEndNodeArray->Add(NewNode);

			return;
		}
	}
	
	if (CurrentNode->HasChild())
	{
		CurrentNode->ForEachChild([this](USkillNode* ChildNode)
		{
			DfsStuffEndNodes(ChildNode);
		});
	}
	else // 如果没有子节点了，也就是遍历到了叶节点
	{
		FSkillNodeInfo NodeInfo;
		NodeInfo.NodeType = ESkillNodeType::LoopEndNode;
		USkillNode* NewNode = NewSkillNode(NodeInfo, USkillNode::StaticClass());
		
		ConnectNode(CurrentNode, NewNode);

		TemporaryLoopEndNodeArray->Add(NewNode);
	}

	// 回溯还原现场
	if (CurrentNode->GetNodeType() == ESkillNodeType::LoopStartNode)
	{
		TemporaryLoopStartNodeBehindTarget.Remove(CurrentNode->GetHashID());
	}
}

USkillsRuntimeSubsystem* USkillsManagerSubsystem::GetSkillsRuntimeSubsystem()
{
	if (!SkillsRuntimeSubsystem)
	{
		SkillsRuntimeSubsystem = GetGameInstance()->GetSubsystem<USkillsRuntimeSubsystem>();
		checkf(SkillsRuntimeSubsystem, TEXT("Millenarysnow : SkillManagerSubsystem cant get SkillRuntimeSubsystem"));
	}
	return SkillsRuntimeSubsystem;
}

void USkillsManagerSubsystem::FlushParentLoopStartNode(USkillNode* _Node)
{
	USkillNode* CurrentNode = _Node;
	while (CurrentNode->GetParentNode())
	{
		CurrentNode = CurrentNode->GetParentNode();
		
		if (CurrentNode->GetNodeType() == ESkillNodeType::LoopStartNode)
			UpdateLoopEndNodes(CurrentNode->GetLoopEndNodesRef(), CurrentNode);
	}
}

void USkillsManagerSubsystem::FlushParentLoopStartNodeWithSelf(USkillNode* _Node)
{
	if (_Node->GetNodeType() == ESkillNodeType::LoopStartNode)
		UpdateLoopEndNodes(_Node->GetLoopEndNodesRef(), _Node);

	FlushParentLoopStartNode(_Node);
}

void USkillsManagerSubsystem::DfsUpdateForwardParam(USkillNode* _Node)
{
	if (!_Node) return;

	// 如果父节点是参数节点，就把信息传下来
	FAccumulativeInfo AccumulativeInfo = FAccumulativeInfo::ZeroValue;
	if (_Node->GetParentNode() && _Node->GetParentNode()->GetNodeType() == ESkillNodeType::ParamNode)
		AccumulativeInfo = _Node->GetParentNode()->GetAccumulativeInfo();

	_Node->SetAccumulativeInfo(_Node->GetAccumulativeInfo() + AccumulativeInfo);

	// 如果已经找到第一个普通节点，处理并返回
	if (_Node->GetNodeType() != ESkillNodeType::ParamNode)
	{
		UpdateTimeDelay(_Node);
		return;
	}
	
	DfsUpdateForwardParam(_Node->GetFirstChildNode());
}

void USkillsManagerSubsystem::UpdateTimeDelay(USkillNode* _UpdateStartNode)
{
	USkillNode* CurrentNode = _UpdateStartNode;
	while (CurrentNode)
	{
		CurrentNode->CalculateDelayTime();
		CurrentNode = CurrentNode->GetParentNode();
	}
}
