// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Skills/ChildNodes/GenerateNode.h"

#include "Game/Gameplay/Component/SkillComponent/ExecuteSkillComponent.h"
#include "Game/Gameplay/Subsytem/Skill/SkillsManagerSubsystem.h"

void UGenerateNode::Ability(const AActor* Target)
{
	if (NodeInfo.NodeType == ESkillNodeType::GenerateNode)
	{
		UExecuteSkillComponent* ExecuteSkillComponent = GetTargetExecuteSkillComponent(Target);
		if (!ExecuteSkillComponent) return;
		
		FNodeGenerateValueFinal GenerateValueFinal;

		GenerateValueFinal.Damage = NodeInfo.Damage * (AccumulativeInfo.AccDamageValue + 1.0f) * USkillsManagerSubsystem::AdditionalParamForAttack;
		GenerateValueFinal.Radius = NodeInfo.Radius * (AccumulativeInfo.RadiusValue + 1.0f);

		ExecuteSkillComponent->GenerateItem(GenerateValueFinal);
	}
}
