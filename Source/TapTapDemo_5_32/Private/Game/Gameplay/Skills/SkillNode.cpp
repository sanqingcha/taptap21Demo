// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Skills/SkillNode.h"

#include "Game/Gameplay/Component/SkillComponent/ExecuteSkillComponent.h"
#include "Game/Gameplay/Subsytem/Skill/SkillsManagerSubsystem.h"

#include "GameFramework/Actor.h"


const FName USkillNode::Tag_SkillComponent = "SkillComponent";

void USkillNode::Initialize(const int32 _HashID, const FSkillNodeInfo& _NodeInfo)
{
	HashID = _HashID;

	NodeInfo = _NodeInfo;

	SetDefaultValue();

	SkillsManagerSubsystem = USkillsManagerSubsystem::Get(this);

	checkf(SkillsManagerSubsystem, TEXT("Millenarysnow : SkillNode cant get SkillManagerSubsystem"));
}

void USkillNode::SetDefaultValue()
{
	AllDelayTime = NodeInfo.DelayTime;
}

bool USkillNode::AddChildNode(USkillNode* Node, OnBranchNode Branch)
{
	if (ChildNodes.Num() >= NodeInfo.OutPinCount) return false;
	
	if (Branch != OnBranchNode::No)
	{
		if (NodeInfo.NodeType != ESkillNodeType::BranchNode) return false;
		
		if (Branch == OnBranchNode::TrU)
		{
			if (!BranchTrueNode) BranchTrueNode = Node;
			else return false;
		}
		else
		{
			if (!BranchFalseNode) BranchFalseNode = Node;
			else return false;			
		}
	}
	
	ChildNodes.Add(Node);

	AllDelayTime = std::max(AllDelayTime, Node->GetDelayTime());
	Node->SetParentNode(this);

	return true;
}

void USkillNode::RemoveChildNode(USkillNode* Node, OnBranchNode Branch)
{
	for (int i = 0; i < ChildNodes.Num(); i++)
	{
		if (ChildNodes[i]->GetHashID() == Node->GetHashID())
		{
			ChildNodes.RemoveAt(i);
			Node->SetParentNode(nullptr);
			break;
		}
	}

	if (Branch != OnBranchNode::No)
	{
		if (Branch == OnBranchNode::TrU) BranchTrueNode = nullptr;
		else BranchFalseNode = nullptr;
	}
	
	CalculateDelayTime();
}

void USkillNode::Delivery(const AActor* Target)
{
	if (!Target) return;
	
	if (NodeInfo.NodeType != ESkillNodeType::BranchNode)
	{
		if (NodeInfo.NodeType == ESkillNodeType::LoopEndNode)
		{
			UExecuteSkillComponent* SkillComponent = Target->FindComponentByTag<UExecuteSkillComponent>(USkillNode::Tag_SkillComponent);
			if (!SkillComponent) return;

			if (LoopStartNode && SkillComponent->HasLoopCountKey(LoopStartNode->GetHashID()) && SkillComponent->GetLoopCount(LoopStartNode->GetHashID()) > 0)
			{
				LoopStartNode->Delivery(Target);
				return;
			}
		}

		for (const auto& Element : ChildNodes)
		{
			Element->Trigger(Target);
		}
	}
	else
	{
		/*
		if (Branch(Target))
		{
			if (BranchTrueNode)
				BranchTrueNode->Trigger(Target);
		}
		else
		{
			if (BranchFalseNode)
				BranchFalseNode->Trigger(Target);
		}
		*/

		for (const auto& Element : ChildNodes)
		{
			Element->Trigger(Target);
		}
	}
}

void USkillNode::Trigger(const AActor* Target)
{
	if (!SkillsManagerSubsystem->GetCanRun()) return;
	
	this->Ability(Target);
	
	if (NodeInfo.NodeType == ESkillNodeType::LoopStartNode)
	{
		UExecuteSkillComponent* SkillComponent = Target->FindComponentByTag<UExecuteSkillComponent>(USkillNode::Tag_SkillComponent);
		if (!SkillComponent) return;
		
		if (!SkillComponent->HasLoopCountKey(GetHashID()))
		{
			SkillComponent->SetLoopCount(GetHashID(), NodeInfo.LoopCount);
		}

		SkillComponent->DecrementLoopCount(GetHashID());

		if (SkillComponent->GetLoopCount(GetHashID()) == 0)
		{
			SkillComponent->DeleteLoopCountKey(GetHashID());
		}
	}
	
	if (NodeInfo.SwitchTargetType == ESwitchTargetType::Unchanged)
		Delivery(Target);
}

bool USkillNode::Branch(const AActor* Target)
{
	return true;
}

int32 USkillNode::CalculateDelayTime()
{
	/**
	 * 参数节点本身不添加累加延迟
	 * 如果不是参数节点，就施加累加减少的延迟
	 */
	float ReduceDelayTimeValue = std::max(1.0f - AccumulativeInfo.AccReduceDelayTimeValue, 0.0f);
	int32 DelayTime = (GetNodeType() == ESkillNodeType::ParamNode) ? 0 : ReduceDelayTimeValue * NodeInfo.DelayTime;

	for (const auto& Element : ChildNodes)
	{
		DelayTime = std::max(DelayTime, Element->GetDelayTime());
	}

	AllDelayTime = DelayTime;

	return AllDelayTime;
}

bool USkillNode::HasChild() const
{
	return ChildNodes.Num() > 0;
}

void USkillNode::SetParentNode(USkillNode* Node)
{
	ParentNode = Node;
}

USkillNode* USkillNode::GetParentNode() const
{
	return ParentNode;
}

int32 USkillNode::GetHashID() const
{
	return HashID;
}

int32 USkillNode::GetDelayTime() const
{
	return AllDelayTime;
}

ESkillNodeType USkillNode::GetNodeType() const
{
	return NodeInfo.NodeType;
}

USkillNode* USkillNode::GetLoopStartNode() const
{
	return LoopStartNode;
}

void USkillNode::SetLoopStartNode(USkillNode* Node)
{
	LoopStartNode = Node;
}

void USkillNode::ForceRemoveAllChild()
{
	for (auto& Element : ChildNodes)
	{
		Element->SetParentNode(nullptr);
	}
	ChildNodes.Empty();

	BranchTrueNode = nullptr;
	BranchFalseNode = nullptr;
}

USkillNode* USkillNode::GetFirstChildNode()
{
	return ChildNodes.Num() > 0 ? ChildNodes[0] : nullptr;
}

FAccumulativeInfo USkillNode::GetAccumulativeInfo() const
{
	return AccumulativeInfo;
}

void USkillNode::SetAccumulativeInfo(const FAccumulativeInfo& Info)
{
	AccumulativeInfo = Info;
}

TArray<USkillNode*>& USkillNode::GetLoopEndNodesRef()
{
	return LoopEndNodes;
}

void USkillNode::SetBranchTrueNode(USkillNode* Node)
{
	BranchTrueNode = Node;
}

void USkillNode::SetBranchFalseNode(USkillNode* Node)
{
	BranchFalseNode = Node;
}

USkillNode* USkillNode::GetBranchTrueNode()
{
	return BranchTrueNode;
}

USkillNode* USkillNode::GetBranchFalseNode()
{
	return BranchFalseNode;
}

const FSkillNodeInfo& USkillNode::GetNodeInfo() const
{
	return NodeInfo;
}

UExecuteSkillComponent* USkillNode::GetTargetExecuteSkillComponent(const AActor* Target) const
{
	if (!Target) return nullptr;
	
	UExecuteSkillComponent* SkillComponent = Target->FindComponentByTag<UExecuteSkillComponent>(USkillNode::Tag_SkillComponent);
	checkf(SkillComponent, TEXT("Millenarysnow : Skill Target not has a Skill Component."));

	return SkillComponent;
}
