// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Skills/SkillNode.h"


const FName USkillNode::Tag_SkillComponent = "SkillComponent";

void USkillNode::Initialize(const int32 _HashID, const FSkillNodeInfo& _NodeInfo)
{
	HashID = _HashID;

	NodeInfo = _NodeInfo;

	SetDefaultValue();
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
	if (NodeInfo.NodeType != ESkillNodeType::BranchNode)
	{
		for (const auto& Element : ChildNodes)
		{
			Element->Trigger(Target);
		}
	}
	else
	{
		if (Branch(Target)) BranchTrueNode->Trigger(Target);
		else BranchFalseNode->Trigger(Target);
	}
}

void USkillNode::Trigger(const AActor* Target)
{
	this->Ability(Target);

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
	 */
	int32 DelayTime = NodeInfo.DelayTime + (GetNodeType() == ESkillNodeType::ParamNode ? 0 : AccumulativeInfo.AccReduceDelayTimeValue);

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

UExecuteSkillComponent* USkillNode::GetTargetExecuteSkillComponent(const AActor* Target) const
{
	if (!Target) return nullptr;
	
	UExecuteSkillComponent* SkillComponent = Target->FindComponentByTag<UExecuteSkillComponent>(USkillNode::Tag_SkillComponent);
	checkf(SkillComponent, TEXT("Millenarysnow : Skill Target not has a Skill Component."));

	return SkillComponent;
}
