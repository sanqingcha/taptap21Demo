// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Subsytem/Skill/SkillsManagerSubsystem.h"

#include "Game/Gameplay/Skills/SkillNode.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

#include <random>
#include <asio/execution/connect.hpp>

void USkillsManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	std::random_device _seed;
	HashGenerateEngine.seed(_seed());
}

USkillNode* USkillsManagerSubsystem::NewSkillNode(const FSkillNodeInfo& NodeInfo)
{
	std::uniform_int_distribution<> distribution(0, 0x3fffffff);
	int32 HashID = distribution(HashGenerateEngine);

	USkillNode* Node = NewObject<USkillNode>(this, USkillNode::StaticClass());
	Node->Initialize(HashID, NodeInfo);

	HashToNode.Add(HashID, Node);

	return Node;
}

void USkillsManagerSubsystem::DeleteSkillNode(USkillNode* Node)
{
	HashToNode.Remove(Node->GetHashID());
	
	Node->ForceRemoveAllChild();

	FlushParentLoopStartNodeWithSelf(Node);
	
	if (Node->GetParentNode())
		DisconnectNode(Node->GetParentNode(), Node);
	
	Node->MarkAsGarbage();
}

bool USkillsManagerSubsystem::ConnectNode(USkillNode* ParentNode, USkillNode* ChildNode)
{
	ParentNode->AddChildNode(ChildNode);
	ChildNode->SetParentNode(ParentNode);
	FlushParentLoopStartNodeWithSelf(ChildNode);
	return true;
}

void USkillsManagerSubsystem::DisconnectNode(USkillNode* ParentNode, USkillNode* ChildNode)
{
	ParentNode->RemoveChildNode(ChildNode);
	FlushParentLoopStartNode(ChildNode);
	ChildNode->SetParentNode(nullptr);
}

void USkillsManagerSubsystem::UpdateLoopEndNodes(TArray<USkillNode*>& LoopEndArray, USkillNode* LoopStartNode)
{
	for (auto& Node : LoopEndArray)
	{
		DeleteSkillNode(Node);
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

void USkillsManagerSubsystem::DfsStuffEndNodes(USkillNode* CurrentNode)
{
	if (!CurrentNode) return;
	
	if (CurrentNode->GetNodeType() == ESkillNodeType::LoopStartNode)
	{
		TemporaryLoopStartNodeBehindTarget.Add(CurrentNode->GetHashID());
	}
	
	if (CurrentNode->GetNodeType() == ESkillNodeType::LoopEndNode)
	{
		int32 LoopStartNodeHashID = CurrentNode->GetLoopStartNode()->GetHashID();
		if (!TemporaryLoopStartNodeBehindTarget.Contains(LoopStartNodeHashID))
		{
			FSkillNodeInfo NodeInfo;
			NodeInfo.NodeType = ESkillNodeType::LoopEndNode;
			USkillNode* NewNode = NewSkillNode(NodeInfo);

			CurrentNode->GetParentNode()->RemoveChildNode(CurrentNode);
			ConnectNode(CurrentNode->GetParentNode(), NewNode);
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
	else
	{
		FSkillNodeInfo NodeInfo;
		NodeInfo.NodeType = ESkillNodeType::LoopEndNode;
		USkillNode* NewNode = NewSkillNode(NodeInfo);
		
		ConnectNode(CurrentNode, NewNode);

		TemporaryLoopEndNodeArray->Add(NewNode);
	}

	if (CurrentNode->GetNodeType() == ESkillNodeType::LoopStartNode)
	{
		TemporaryLoopStartNodeBehindTarget.Remove(CurrentNode->GetHashID());
	}
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

void USkillsManagerSubsystem::FlushParentLoopStartNodeWithSelf(USkillNode* Node)
{
	if (Node->GetNodeType() == ESkillNodeType::LoopStartNode)
		UpdateLoopEndNodes(Node->GetLoopEndNodesRef(), Node);

	FlushParentLoopStartNode(Node);
}

