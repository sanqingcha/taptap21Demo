// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillFragment.h"
#include "SkillFragmentDamageModifier.generated.h"

/**
 * 伤害修改 Fragment
 * 功能：使目标伤害提升或减少（Type.Strength > 1 提升，< 1 减少）
 */
UCLASS(Blueprintable, BlueprintType)
class TAPTAPDEMO_5_32_API USkillFragment_DamageModifier : public USkillFragment
{
	GENERATED_BODY()
public:
	/** 检测半径（会被 Type.Radius 修正） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	float BaseDetectionRadius = 500.0f;

	/** 检测通道 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	TEnumAsByte<ECollisionChannel> DetectionChannel = ECC_Pawn;

	/** 是否忽略触发源 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	bool bIgnoreSource = false;

	/** 伤害修改 GE（Duration 类型，使用 SetByCaller 接收 Type.Strength） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<class UGameplayEffect> DamageModifierEffectClass;

	/** 应用特效（可选） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	class UNiagaraSystem *ApplyEffect;

	/** 应用音效（可选） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	class USoundBase *ApplySound;

	/** 是否显示调试球体 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bShowDebugSphere = false;

	virtual void OnTrrigger_Implementation(UPlayerBaseData *Data, const FVector &Trrigger,
										   const FNodeBuffValueFinal &Type, AActor *Source) override;

private:
	/** 检测范围内的目标并应用 GE */
	void DetectAndApplyDamageModifier(UPlayerBaseData *Data, const FVector &Location,
									  AActor *Source, const FNodeBuffValueFinal &Type);

	/** 应用伤害修改 GE */
	void ApplyDamageModifierToTarget(UPlayerBaseData *Data, AActor *SourceActor,
									 AActor *TargetActor, const FNodeBuffValueFinal &Type);

	/** 生成视觉反馈 */
	void SpawnVisualFeedback(UWorld *World, const FVector &Location);
};
