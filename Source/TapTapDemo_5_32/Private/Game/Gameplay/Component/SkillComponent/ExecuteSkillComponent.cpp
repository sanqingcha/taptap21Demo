// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Component/SkillComponent/ExecuteSkillComponent.h"

#include "Game/Gameplay/Skills/SkillNode.h"

UExecuteSkillComponent::UExecuteSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	this->ComponentTags.Add(USkillNode::Tag_SkillComponent);
}


void UExecuteSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UExecuteSkillComponent::TriggerTargetSkillList(const AActor* TargetActor)
{
	CurrentSkillNode->Delivery(TargetActor);
}

bool UExecuteSkillComponent::IfBloodLow() const
{
	return false;
}

bool UExecuteSkillComponent::IfInAir() const
{
	return false;
}

void UExecuteSkillComponent::EffectBuff(FNodeBuffValueFinal BuffValueFinal)
{
}

void UExecuteSkillComponent::GenerateItem(FNodeGenerateValueFinal GenerateValueFinal)
{
}



