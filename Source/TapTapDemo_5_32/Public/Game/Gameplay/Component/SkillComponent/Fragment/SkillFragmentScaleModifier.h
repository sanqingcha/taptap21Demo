// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillFragment.h"
#include "SkillFragmentScaleModifier.generated.h"

/**
 * 缩放修改 Fragment
 * 功能：使目标放大或缩小（Type.Radius > 1 放大，< 1 缩小）
 * 注意：这个 Fragment 直接修改 Actor 的 Scale，同时应用一个标记 GE 来跟踪 Buff 时长
 */
UCLASS(Blueprintable, BlueprintType)
class TAPTAPDEMO_5_32_API USkillFragment_ScaleModifier : public USkillFragment
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

	/** 缩放标记 GE（Duration 类型，用于追踪持续时间并在结束时恢复缩放） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<class UGameplayEffect> ScaleModifierEffectClass;

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
	/** 检测范围内的目标并应用缩放 */
	void DetectAndApplyScaleModifier(UPlayerBaseData *Data, const FVector &Location,
									 AActor *Source, const FNodeBuffValueFinal &Type);

	/** 应用缩放修改 */
	void ApplyScaleModifierToTarget(UPlayerBaseData *Data, AActor *SourceActor,
									AActor *TargetActor, const FNodeBuffValueFinal &Type);

	/** 生成视觉反馈 */
	void SpawnVisualFeedback(UWorld *World, const FVector &Location);
};
