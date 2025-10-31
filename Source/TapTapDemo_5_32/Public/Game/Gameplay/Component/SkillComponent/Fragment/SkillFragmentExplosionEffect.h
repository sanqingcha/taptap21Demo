// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillFragment.h"
#include "SkillFragmentExplosionEffect.generated.h"

/**
 * 爆炸范围伤害/治疗
 * 功能：在触发位置生成爆炸特效，对范围内目标应用伤害或治疗 GameplayEffect
 */
UCLASS(Blueprintable, BlueprintType)
class TAPTAPDEMO_5_32_API USkillFragment_ExplosionEffect : public USkillFragment
{
	GENERATED_BODY()
public:
	/** 爆炸基础半径（会被 Type.Radius 修正） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float BaseExplosionRadius = 500.0f;

	/** 爆炸特效 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion|VFX")
	class UNiagaraSystem *ExplosionEffect;

	/** 爆炸音效 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion|Audio")
	class USoundBase *ExplosionSound;

	/** 相机震动 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion|Feedback")
	TSubclassOf<class UCameraShakeBase> CameraShakeClass;

	/** 相机震动范围 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion|Feedback")
	float CameraShakeRadius = 1000.0f;

	/** 是否显示调试球体 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion|Debug")
	bool bShowDebugSphere = false;

	/** 对敌人应用的伤害 GE */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<class UGameplayEffect> DamageEffectClass;

	/** 对友方应用的治疗 GE */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<class UGameplayEffect> HealEffectClass;

	/** 是否也对触发源应用治疗 GE */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	bool bHealSource = true;

	// ==================== 目标检测配置 ====================

	/** 检测通道（默认 Pawn） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	TEnumAsByte<ECollisionChannel> DetectionChannel = ECC_Pawn;

	/** 是否忽略触发源 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	bool bIgnoreSource = true;

	// ==================== 核心函数 ====================

	virtual void OnTrrigger_Implementation(UPlayerBaseData *Data, const FVector &Trrigger,
										   const FNodeBuffValueFinal &Type, AActor *Source) override;

private:
	/** 获取真正的玩家 Pawn（处理 Source 是发射物的情况） */
	APawn *GetPlayerPawn(AActor *Source) const;

	/** 生成爆炸特效 */
	void SpawnExplosionEffects(UWorld *World, const FVector &Location);

	/** 检测范围内的目标并应用 GE */
	void DetectAndApplyEffects(UPlayerBaseData *Data, const FVector &Location,
							   AActor *PlayerPawn, const FNodeBuffValueFinal &Type);

	/** 判断目标是否是友方 */
	bool IsAlly(AActor *Target, AActor *Source) const;

	/** 应用 GameplayEffect 到目标（使用 Type 参数） */
	void ApplyGameplayEffectToTarget(UPlayerBaseData *Data, AActor *SourceActor,
									 AActor *TargetActor, TSubclassOf<UGameplayEffect> GEClass,
									 const FNodeBuffValueFinal &Type);
};
