// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "Game/Gameplay/Skills/SkillNode.h"

#include "ExecuteSkillComponent.generated.h"


class UNiagaraSystem;
class ACallBackProjectileActor;
class UGameplayEffect;
class USkillNode;

USTRUCT(BlueprintType)
struct FGenerateData
{
	GENERATED_BODY()
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//float BoomRadiu = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> BoomGE;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> NS; 
		
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<ACallBackProjectileActor> ProjectileClass;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TAPTAPDEMO_5_32_API UExecuteSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UExecuteSkillComponent();

#pragma region SkillInterface

	// 血量是否低于阈值
	virtual bool IfBloodLow();

	// 是否未接触地面
	virtual bool IfInAir() const;

	// 施加增益
	virtual void EffectBuff(FNodeBuffValueFinal BuffValueFinal);

	// 生成生成物
	virtual void GenerateItem(FNodeGenerateValueFinal GenerateValueFinal);

#pragma endregion

#pragma region UTIL

	void SetCurrentSkillNode(USkillNode* Node);

	void SetLoopCount(int32 LoopHashID, int32 LoopCount);
	int32 GetLoopCount(int32 LoopHashID);
	void DecrementLoopCount(int32 LoopHashID);
	bool HasLoopCountKey(int32 LoopHashID);
	void DeleteLoopCountKey(int32 LoopHashID);
	void ClearEachLoopCountMap();
	
#pragma endregion

protected:
	virtual void BeginPlay() override;
	
	// 获取到目标对象后必须手动调用，否则无法触发后续技能链
	void TriggerTargetSkillList(const AActor* TargetActor);

private:
	UPROPERTY()
	USkillNode* CurrentSkillNode;

	UPROPERTY()
	TMap<int32, int32> EachLoopCount;
};

UCLASS(Blueprintable)
class TAPTAPDEMO_5_32_API USkillComponent : public UExecuteSkillComponent
{
 	GENERATED_BODY()
	USkillComponent();
	public:
#pragma region SkillInterface

	// 血量是否低于阈值
	virtual bool IfBloodLow();

	// 是否未接触地面
	virtual bool IfInAir() const;

	// 施加增益
	virtual void EffectBuff(FNodeBuffValueFinal BuffValueFinal);

	// 生成生成物
	virtual void GenerateItem(FNodeGenerateValueFinal GenerateValueFinal);

#pragma endregion


	void CallBackOnHit(AActor* Target);
	void SpawnProjectile(FNodeGenerateValueFinal GenerateValueFinal);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGenerateData GenerateData;
	

};
