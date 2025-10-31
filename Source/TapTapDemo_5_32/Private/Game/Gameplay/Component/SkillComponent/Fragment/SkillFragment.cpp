// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/Gameplay/Component/SkillComponent/Fragment/SkillFragment.h"


void USkillFragment::OnTrrigger_Implementation(UPlayerBaseData *Data, const FVector &Trrigger, const FNodeBuffValueFinal &Type, AActor *Source)
{

}

void USkillFragment_ApplyDatageRange::OnTrrigger_Implementation(UPlayerBaseData *Data, const FVector &Trrigger, const FNodeBuffValueFinal &Type, AActor *Source)
{
	Super::OnTrrigger_Implementation(Data, Trrigger, Type, Source);
	/**
	 * 范围检测到敌人->检测后应用伤害
	 */
}
