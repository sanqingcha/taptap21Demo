// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Game/Gameplay/Skills/SkillNode.h"

#include "ExecuteSkillComponent.generated.h"


class USkillNode;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TAPTAPDEMO_5_32_API UExecuteSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UExecuteSkillComponent();

	// 获取到目标对象后必须手动调用，否则无法触发后续技能链
	virtual void TriggerTargetSkillList(const AActor* TargetActor);

#pragma region SkillInterface

	// 血量是否低于阈值
	virtual bool IfBloodLow() const;

	// 是否未接触地面
	virtual bool IfInAir() const;

	// 施加增益
	virtual void EffectBuff(FNodeBuffValueFinal BuffValueFinal);

	// 生成生成物
	virtual void GenerateItem(FNodeGenerateValueFinal GenerateValueFinal);

#pragma endregion

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	USkillNode* CurrentSkillNode;
};
