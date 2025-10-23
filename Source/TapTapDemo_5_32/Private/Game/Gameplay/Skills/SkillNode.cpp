// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Skills/SkillNode.h"


void USkillNode::Initialize(const int32 _HashID, const FSkillNodeInfo& _NodeInfo)
{
	HashID = _HashID;

	NodeInfo = _NodeInfo;

	SetDefaultValue();
}

void USkillNode::SetDefaultValue()
{
}

void USkillNode::AddChildNode(USkillNode* Node)
{
	ChildNodes.Add(Node);

	AllDelayTime = std::max(AllDelayTime, Node->GetDelayTime());
	Node->SetParentNode(this);
}

void USkillNode::RemoveChildNode(USkillNode* Node)
{
	AllDelayTime = NodeInfo.DelayTime;
	
	for (int i = 0; i < ChildNodes.Num(); i++)
	{
		if (ChildNodes[i]->GetHashID() == Node->GetHashID())
		{
			ChildNodes.RemoveAt(i);
			Node->SetParentNode(nullptr);
			break;
		}
	}
	
	for (const auto& Element : ChildNodes)
		AllDelayTime = std::max(AllDelayTime, Element->GetDelayTime());
}

void USkillNode::Trigger(const UObject* Target)
{
	this->Ability();
}

bool USkillNode::Branch()
{
	return true;
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
}

USkillNode* USkillNode::GetFirstChildNode()
{
	return ChildNodes.Num() > 0 ? ChildNodes[0] : nullptr;
}

TArray<USkillNode*>& USkillNode::GetLoopEndNodesRef()
{
	return LoopEndNodes;
}
