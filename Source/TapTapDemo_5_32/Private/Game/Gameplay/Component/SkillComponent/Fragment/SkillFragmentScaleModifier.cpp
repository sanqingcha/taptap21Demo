// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/Gameplay/Component/SkillComponent/Fragment/SkillFragmentScaleModifier.h"

#include "Game/Gameplay/Player/Data/PlayerBaseData.h"
#include "Game/Gameplay/Subsytem/GISubSystem/SoundSettingSubsystem.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"

void USkillFragment_ScaleModifier::OnTrrigger_Implementation(
	UPlayerBaseData *Data,
	const FVector &Trrigger,
	const FNodeBuffValueFinal &Type,
	AActor *Source)
{
	if (!Data || !Source)
	{
		UE_LOG(LogTemp, Warning, TEXT("USkillFragment_ScaleModifier: Invalid Data or Source"));
		return;
	}

	UWorld *World = Source->GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("USkillFragment_ScaleModifier: Invalid World"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("USkillFragment_ScaleModifier: Triggered at %s, Scale: %.2f%%"),
		   *Trrigger.ToString(), Type.Radius * 100.0f);

	// 1. 生成视觉反馈
	SpawnVisualFeedback(World, Trrigger);

	// 2. 检测并应用缩放修改
	DetectAndApplyScaleModifier(Data, Trrigger, Source, Type);

	// 3. 调试可视化
	if (bShowDebugSphere)
	{
		float FinalRadius = BaseDetectionRadius * Type.Radius;
		FColor DebugColor = Type.Radius > 1.0f ? FColor::Yellow : FColor::Cyan;
		DrawDebugSphere(World, Trrigger, FinalRadius, 32, DebugColor, false, Type.BuffDurationTime, 0, 2.0f);

		FString DebugText = FString::Printf(TEXT("Scale: %.0f%% for %.1fs"),
											Type.Radius * 100.0f, Type.BuffDurationTime);
		DrawDebugString(World, Trrigger + FVector(0, 0, 100), DebugText, nullptr, FColor::White,
						Type.BuffDurationTime, true);
	}
}

void USkillFragment_ScaleModifier::DetectAndApplyScaleModifier(
	UPlayerBaseData *Data,
	const FVector &Location,
	AActor *Source,
	const FNodeBuffValueFinal &Type)
{
	if (!Data || !Data->ASC || !Source)
	{
		return;
	}

	UWorld *World = Source->GetWorld();
	if (!World)
	{
		return;
	}

	// 计算修正后的半径（用于检测，不是缩放值）
	float DetectionRadius = BaseDetectionRadius;

	// 范围检测目标
	TArray<FHitResult> HitResults;
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(DetectionRadius);
	FCollisionQueryParams QueryParams;

	if (bIgnoreSource)
	{
		QueryParams.AddIgnoredActor(Source);
	}

	bool bHit = World->SweepMultiByChannel(
		HitResults,
		Location,
		Location + FVector(0, 0, 1),
		FQuat::Identity,
		DetectionChannel,
		SphereShape,
		QueryParams);

	if (!bHit)
	{
		UE_LOG(LogTemp, Log, TEXT("USkillFragment_ScaleModifier: No targets found in radius (%.0f)"), DetectionRadius);
		return;
	}

	// 应用缩放修改到每个目标
	int32 AffectedCount = 0;
	for (const FHitResult &Hit : HitResults)
	{
		AActor *HitActor = Hit.GetActor();
		if (!HitActor || HitActor == Source)
		{
			continue;
		}

		// 检查目标是否有 ASC
		UAbilitySystemComponent *TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
		if (!TargetASC)
		{
			UE_LOG(LogTemp, Warning, TEXT("USkillFragment_ScaleModifier: Target %s has no ASC"),
				   *HitActor->GetName());
			continue;
		}

		ApplyScaleModifierToTarget(Data, Source, HitActor, Type);
		AffectedCount++;
	}

	UE_LOG(LogTemp, Log, TEXT("USkillFragment_ScaleModifier: Applied scale modifier to %d targets"), AffectedCount);
}

void USkillFragment_ScaleModifier::ApplyScaleModifierToTarget(
	UPlayerBaseData *Data,
	AActor *SourceActor,
	AActor *TargetActor,
	const FNodeBuffValueFinal &Type)
{
	if (!Data || !Data->ASC || !SourceActor || !TargetActor)
	{
		return;
	}

	UAbilitySystemComponent *TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)
	{
		return;
	}

	// 1. 保存原始缩放（如果还没有保存的话）
	FVector OriginalScale = TargetActor->GetActorScale3D();

	// 2. 应用新的缩放
	FVector NewScale = OriginalScale * Type.Radius;
	TargetActor->SetActorScale3D(NewScale);

	UE_LOG(LogTemp, Log, TEXT("USkillFragment_ScaleModifier: Scaled %s from %s to %s"),
		   *TargetActor->GetName(), *OriginalScale.ToString(), *NewScale.ToString());

	// 3. 应用标记 GE 来追踪持续时间（可选）
	if (ScaleModifierEffectClass)
	{
		// 创建 GE Context
		FGameplayEffectContextHandle EffectContext = Data->ASC->MakeEffectContext();
		EffectContext.AddSourceObject(SourceActor);
		EffectContext.AddInstigator(SourceActor, SourceActor);

		// 创建 GE Spec
		FGameplayEffectSpecHandle SpecHandle = Data->ASC->MakeOutgoingSpec(
			ScaleModifierEffectClass,
			1.0f,
			EffectContext);

		if (SpecHandle.IsValid())
		{
			// 传递参数
			SpecHandle.Data->SetSetByCallerMagnitude(
				FGameplayTag::RequestGameplayTag(FName("Data.Radius")),
				Type.Radius);

			SpecHandle.Data->SetSetByCallerMagnitude(
				FGameplayTag::RequestGameplayTag(FName("Data.BuffDuration")),
				Type.BuffDurationTime);

			// 应用 GE
			FActiveGameplayEffectHandle ActiveHandle = Data->ASC->ApplyGameplayEffectSpecToTarget(
				*SpecHandle.Data.Get(),
				TargetASC);

			if (ActiveHandle.IsValid())
			{
				// 4. 设置定时器，在 Buff 结束时恢复缩放
				FTimerHandle TimerHandle;
				TargetActor->GetWorldTimerManager().SetTimer(
					TimerHandle,
					[TargetActor, OriginalScale]()
					{
						if (TargetActor && TargetActor->IsValidLowLevel())
						{
							TargetActor->SetActorScale3D(OriginalScale);
							UE_LOG(LogTemp, Log, TEXT("USkillFragment_ScaleModifier: Restored scale of %s to %s"),
								   *TargetActor->GetName(), *OriginalScale.ToString());
						}
					},
					Type.BuffDurationTime,
					false);

				UE_LOG(LogTemp, Log, TEXT("USkillFragment_ScaleModifier: Applied GE to %s (%.0f%% for %.1fs)"),
					   *TargetActor->GetName(), Type.Radius * 100.0f, Type.BuffDurationTime);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("USkillFragment_ScaleModifier: Failed to apply GE, scale will not auto-restore"));
			}
		}
	}
	else
	{
		// 没有 GE，直接使用定时器恢复
		FTimerHandle TimerHandle;
		TargetActor->GetWorldTimerManager().SetTimer(
			TimerHandle,
			[TargetActor, OriginalScale]()
			{
				if (TargetActor && TargetActor->IsValidLowLevel())
				{
					TargetActor->SetActorScale3D(OriginalScale);
					UE_LOG(LogTemp, Log, TEXT("USkillFragment_ScaleModifier: Restored scale of %s to %s"),
						   *TargetActor->GetName(), *OriginalScale.ToString());
				}
			},
			Type.BuffDurationTime,
			false);
	}
}

void USkillFragment_ScaleModifier::SpawnVisualFeedback(UWorld *World, const FVector &Location)
{
	if (!World)
	{
		return;
	}

	// 生成 Niagara 特效
	if (ApplyEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			World,
			ApplyEffect,
			Location,
			FRotator::ZeroRotator,
			FVector(1.0f),
			true,
			true,
			ENCPoolMethod::None,
			true);
	}

	// 播放音效
	if (ApplySound)
	{
		float VolumeMultiplier = 1.0f;

		if (UGameInstance *GameInstance = World->GetGameInstance())
		{
			if (UGameSettingSubsystem *SoundSettings = GameInstance->GetSubsystem<UGameSettingSubsystem>())
			{
				VolumeMultiplier = SoundSettings->GetSoundSettings().SoundEffectVolume;
			}
		}

		UGameplayStatics::PlaySoundAtLocation(
			World,
			ApplySound,
			Location,
			VolumeMultiplier,
			1.0f,
			0.0f,
			nullptr,
			nullptr,
			nullptr);
	}
}
