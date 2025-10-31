// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/Gameplay/Component/SkillComponent/Fragment/SkillFragmentDamageModifier.h"

#include "Game/Gameplay/Player/Data/PlayerBaseData.h"
#include "Game/Gameplay/Subsytem/GISubSystem/SoundSettingSubsystem.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

void USkillFragment_DamageModifier::OnTrrigger_Implementation(
	UPlayerBaseData *Data,
	const FVector &Trrigger,
	const FNodeBuffValueFinal &Type,
	AActor *Source)
{
	if (!Data || !Source)
	{
		UE_LOG(LogTemp, Warning, TEXT("USkillFragment_DamageModifier: Invalid Data or Source"));
		return;
	}

	UWorld *World = Source->GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("USkillFragment_DamageModifier: Invalid World"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("USkillFragment_DamageModifier: Triggered at %s, Strength: %.2f%%"),
		   *Trrigger.ToString(), Type.Strength * 100.0f);

	// 1. 生成视觉反馈
	SpawnVisualFeedback(World, Trrigger);

	// 2. 检测并应用伤害修改
	DetectAndApplyDamageModifier(Data, Trrigger, Source, Type);

	// 3. 调试可视化
	if (bShowDebugSphere)
	{
		float FinalRadius = BaseDetectionRadius * Type.Radius;
		FColor DebugColor = Type.Strength > 1.0f ? FColor::Red : FColor::Purple;
		DrawDebugSphere(World, Trrigger, FinalRadius, 32, DebugColor, false, Type.BuffDurationTime, 0, 2.0f);

		FString DebugText = FString::Printf(TEXT("Damage: %.0f%% for %.1fs"),
											Type.Strength * 100.0f, Type.BuffDurationTime);
		DrawDebugString(World, Trrigger + FVector(0, 0, 100), DebugText, nullptr, FColor::White,
						Type.BuffDurationTime, true);
	}
}

void USkillFragment_DamageModifier::DetectAndApplyDamageModifier(
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

	// 计算修正后的半径
	float FinalRadius = BaseDetectionRadius * Type.Radius;

	// 范围检测目标
	TArray<FHitResult> HitResults;
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(FinalRadius);
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
		UE_LOG(LogTemp, Log, TEXT("USkillFragment_DamageModifier: No targets found in radius (%.0f)"), FinalRadius);
		return;
	}

	// 应用伤害修改到每个目标
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
			UE_LOG(LogTemp, Warning, TEXT("USkillFragment_DamageModifier: Target %s has no ASC"),
				   *HitActor->GetName());
			continue;
		}

		ApplyDamageModifierToTarget(Data, Source, HitActor, Type);
		AffectedCount++;
	}

	UE_LOG(LogTemp, Log, TEXT("USkillFragment_DamageModifier: Applied damage modifier to %d targets"), AffectedCount);
}

void USkillFragment_DamageModifier::ApplyDamageModifierToTarget(
	UPlayerBaseData *Data,
	AActor *SourceActor,
	AActor *TargetActor,
	const FNodeBuffValueFinal &Type)
{
	if (!Data || !Data->ASC || !SourceActor || !TargetActor || !DamageModifierEffectClass)
	{
		return;
	}

	UAbilitySystemComponent *TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)
	{
		return;
	}

	// 创建 GE Context
	FGameplayEffectContextHandle EffectContext = Data->ASC->MakeEffectContext();
	EffectContext.AddSourceObject(SourceActor);
	EffectContext.AddInstigator(SourceActor, SourceActor);

	// 创建 GE Spec
	FGameplayEffectSpecHandle SpecHandle = Data->ASC->MakeOutgoingSpec(
		DamageModifierEffectClass,
		1.0f,
		EffectContext);

	if (!SpecHandle.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("USkillFragment_DamageModifier: Failed to create GE Spec"));
		return;
	}

	// 传递参数
	SpecHandle.Data->SetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Data.Strength")),
		Type.Strength);

	SpecHandle.Data->SetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Data.BuffDuration")),
		Type.BuffDurationTime);

	// 应用 GE
	FActiveGameplayEffectHandle ActiveHandle = Data->ASC->ApplyGameplayEffectSpecToTarget(
		*SpecHandle.Data.Get(),
		TargetASC);

	if (ActiveHandle.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("USkillFragment_DamageModifier: Applied to %s (%.0f%% for %.1fs)"),
			   *TargetActor->GetName(), Type.Strength * 100.0f, Type.BuffDurationTime);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("USkillFragment_DamageModifier: Failed to apply to %s"),
			   *TargetActor->GetName());
	}
}

void USkillFragment_DamageModifier::SpawnVisualFeedback(UWorld *World, const FVector &Location)
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
