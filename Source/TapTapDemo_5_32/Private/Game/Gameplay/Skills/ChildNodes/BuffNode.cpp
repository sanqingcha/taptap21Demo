// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Skills/ChildNodes/BuffNode.h"

#include "Game/Gameplay/Component/SkillComponent/ExecuteSkillComponent.h"
#include "Game/Gameplay/Subsytem/Skill/SkillsManagerSubsystem.h"

void UBuffNode::Ability(const AActor* Target)
{
	if (NodeInfo.NodeType == ESkillNodeType::BuffNode)
	{
		FNodeBuffValueFinal BuffValueFinal;

		BuffValueFinal.BuffType = NodeInfo.BuffType;
		BuffValueFinal.BuffDurationTime = NodeInfo.BuffDurationTime * (AccumulativeInfo.AccEffectiveTimeValue + 1.0f);
		BuffValueFinal.Strength = NodeInfo.BuffValue * (AccumulativeInfo.AccDamageValue + 1.0f) * USkillsManagerSubsystem::AdditionalParamForAttack;
		BuffValueFinal.Radius = NodeInfo.BuffValue * (AccumulativeInfo.RadiusValue + 1.0f);
		BuffValueFinal.Accelerate = NodeInfo.BuffValue * (AccumulativeInfo.AccReduceDelayTimeValue + 1.0f);

		UExecuteSkillComponent* SkillComponent = GetTargetExecuteSkillComponent(Target);
		if (SkillComponent)
			SkillComponent->EffectBuff(BuffValueFinal);
	}
}
