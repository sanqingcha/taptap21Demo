// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Skills/ChildNodes/BranchNode.h"

#include "Game/Gameplay/Component/SkillComponent/ExecuteSkillComponent.h"

bool UBranchNode::Branch(const AActor* Target)
{
	if (NodeInfo.NodeType == ESkillNodeType::BranchNode)
	{
		UExecuteSkillComponent* SkillComponent = GetTargetExecuteSkillComponent(Target);
		if (!SkillComponent) return false;
		
		switch (NodeInfo.BranchType)
		{
		case EBranchType::BloodLow:
			{
				return SkillComponent->IfBloodLow();
			}
		case EBranchType::InAir:
			{
				return SkillComponent->IfInAir();
			}
		case EBranchType::None:
			{
				return false;
			}
		default:
			return false;
		}
	}
	else
	{
		return true;
	}
}
