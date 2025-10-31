// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/Gameplay/Component/SkillComponent/Fragment/SkillFragmentExplosionEffect.h"

#include "Game/Gameplay/Player/Data/PlayerBaseData.h"
#include "Game/GameplayTags/GameTags.h"
#include "Game/Gameplay/Subsytem/GISubSystem/SoundSettingSubsystem.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraShakeBase.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Pawn.h"

void USkillFragment_ExplosionEffect::OnTrrigger_Implementation(
	UPlayerBaseData *Data,
	const FVector &Trrigger,
	const FNodeBuffValueFinal &Type,
	AActor *Source)
{
	if (!Data || !Source)
	{
		UE_LOG(LogTemp, Warning, TEXT("USkillFragment_ExplosionEffect: Invalid Data or Source"));
		return;
	}

	UWorld *World = Source->GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("USkillFragment_ExplosionEffect: Invalid World"));
		return;
	}

	// 1. 获取真正的玩家 Pawn
	APawn *PlayerPawn = GetPlayerPawn(Source);
	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("USkillFragment_ExplosionEffect: Cannot find Player Pawn"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("USkillFragment_ExplosionEffect: Triggered at %s by %s (Player: %s)"),
		   *Trrigger.ToString(), *Source->GetName(), *PlayerPawn->GetName());
	UE_LOG(LogTemp, Log, TEXT("  Type - Strength: %.2f, Radius: %.2f, Accelerate: %.2f, Duration: %.2f"),
		   Type.Strength, Type.Radius, Type.Accelerate, Type.BuffDurationTime);

	// 2. 生成爆炸特效
	SpawnExplosionEffects(World, Trrigger);

	// 3. 应用 GE
	DetectAndApplyEffects(Data, Trrigger, PlayerPawn, Type);

	if (bShowDebugSphere)
	{
		// 使用 Type.Radius 修正后的半径
		float FinalRadius = BaseExplosionRadius * Type.Radius;
		DrawDebugSphere(World, Trrigger, FinalRadius, 32, FColor::Red, false, 3.0f, 0, 2.0f);

		FString DebugText = FString::Printf(TEXT("Radius: %.0f (%.2fx)"), FinalRadius, Type.Radius);
		DrawDebugString(World, Trrigger + FVector(0, 0, 100), DebugText, nullptr, FColor::White, 3.0f, true);
	}
}

APawn *USkillFragment_ExplosionEffect::GetPlayerPawn(AActor *Source) const
{
	if (!Source)
	{
		return nullptr;
	}

	// 情况1：Source 本身就是 Pawn（玩家直接触发）
	if (APawn *SourcePawn = Cast<APawn>(Source))
	{
		return SourcePawn;
	}

	// 情况2：Source 是发射物，通过 Instigator 获取玩家
	if (APawn *InstigatorPawn = Source->GetInstigator())
	{
		return InstigatorPawn;
	}

	// 情况3：Source 是发射物，通过 Owner 获取玩家
	if (AActor *Owner = Source->GetOwner())
	{
		if (APawn *OwnerPawn = Cast<APawn>(Owner))
		{
			return OwnerPawn;
		}

		// Owner 可能也不是 Pawn，继续查找 Owner 的 Instigator
		if (APawn *OwnerInstigator = Owner->GetInstigator())
		{
			return OwnerInstigator;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("USkillFragment_ExplosionEffect: Cannot find Player Pawn from Source %s"),
		   *Source->GetName());
	return nullptr;
}

void USkillFragment_ExplosionEffect::SpawnExplosionEffects(UWorld *World, const FVector &Location)
{
	if (!World)
	{
		return;
	}

	// 生成 Niagara 特效
	if (ExplosionEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			World,
			ExplosionEffect,
			Location,
			FRotator::ZeroRotator,
			FVector(1.0f),
			true,
			true,
			ENCPoolMethod::None,
			true);

		UE_LOG(LogTemp, Log, TEXT("USkillFragment_ExplosionEffect: Spawned Niagara effect at %s"),
			   *Location.ToString());
	}

	// 播放音效（使用音量管理）
	if (ExplosionSound)
	{
		float VolumeMultiplier = 1.0f;

		// 从 GameInstance 获取音量设置
		if (UGameInstance *GameInstance = World->GetGameInstance())
		{
			if (UGameSettingSubsystem *SoundSettings = GameInstance->GetSubsystem<UGameSettingSubsystem>())
			{
				VolumeMultiplier = SoundSettings->GetSoundSettings().SoundEffectVolume;
			}
		}

		UGameplayStatics::PlaySoundAtLocation(
			World,
			ExplosionSound,
			Location,
			VolumeMultiplier, // 使用音量设置
			1.0f,	 // Pitch Multiplier
			0.0f,	 // Start Time
			nullptr, // Attenuation Settings
			nullptr, // Concurrency Settings
			nullptr	 // Owner
		);
	}

	// 相机震动
	if (CameraShakeClass)
	{
		UGameplayStatics::PlayWorldCameraShake(
			World,
			CameraShakeClass,
			Location,
			0.0f,			   // Inner Radius
			CameraShakeRadius, // Outer Radius
			1.0f,			   // Falloff
			false			   // bOrientShakeTowardsEpicenter
		);
	}
}

void USkillFragment_ExplosionEffect::DetectAndApplyEffects(
	UPlayerBaseData *Data,
	const FVector &Location,
	AActor *PlayerPawn,
	const FNodeBuffValueFinal &Type)
{
	if (!Data || !Data->ASC || !PlayerPawn)
	{
		return;
	}

	UWorld *World = PlayerPawn->GetWorld();
	if (!World)
	{
		return;
	}

	// 计算修正后的半径
	float FinalRadius = BaseExplosionRadius * Type.Radius;

	// 范围检测目标
	TArray<FHitResult> HitResults;
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(FinalRadius);
	FCollisionQueryParams QueryParams;

	if (bIgnoreSource)
	{
		QueryParams.AddIgnoredActor(PlayerPawn);
	}

	bool bHit = World->SweepMultiByChannel(
		HitResults,
		Location,
		Location + FVector(0, 0, 1), // 微小偏移避免精度问题
		FQuat::Identity,
		DetectionChannel,
		SphereShape,
		QueryParams);

	if (!bHit)
	{
		UE_LOG(LogTemp, Log, TEXT("USkillFragment_ExplosionEffect: No targets found in explosion radius (%.0f)"),
			   FinalRadius);

		// 没有检测到目标，但可能需要治疗触发源
		if (bHealSource && HealEffectClass)
		{
			ApplyGameplayEffectToTarget(Data, PlayerPawn, PlayerPawn, HealEffectClass, Type);
		}
		return;
	}

	// 对每个命中的目标应用 GE
	int32 DamagedCount = 0;
	int32 HealedCount = 0;

	for (const FHitResult &Hit : HitResults)
	{
		AActor *HitActor = Hit.GetActor();
		if (!HitActor || HitActor == PlayerPawn)
		{
			continue;
		}

		// 检查目标是否有 ASC
		UAbilitySystemComponent *TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
		if (!TargetASC)
		{
			UE_LOG(LogTemp, Warning, TEXT("USkillFragment_ExplosionEffect: Target %s has no ASC"),
				   *HitActor->GetName());
			continue;
		}

		// 根据目标类型应用不同的 GE
		if (IsAlly(HitActor, PlayerPawn))
		{
			// 友方 -> 治疗
			if (HealEffectClass)
			{
				ApplyGameplayEffectToTarget(Data, PlayerPawn, HitActor, HealEffectClass, Type);
				HealedCount++;
			}
		}
		else
		{
			// 敌方 -> 伤害
			if (DamageEffectClass)
			{
				ApplyGameplayEffectToTarget(Data, PlayerPawn, HitActor, DamageEffectClass, Type);
				DamagedCount++;
			}
		}
	}

	// 治疗触发源（可选）
	if (bHealSource && HealEffectClass)
	{
		ApplyGameplayEffectToTarget(Data, PlayerPawn, PlayerPawn, HealEffectClass, Type);
		HealedCount++;
	}

	UE_LOG(LogTemp, Log, TEXT("USkillFragment_ExplosionEffect: Damaged %d enemies, Healed %d allies"),
		   DamagedCount, HealedCount);
}

// 判断目标是否是友方
bool USkillFragment_ExplosionEffect::IsAlly(AActor *Target, AActor *Source) const
{
	if (!Target || !Source)
	{
		return false;
	}

	// 方法1：使用 GameplayTag 检查（修改1：使用项目的 GameplayTag 系统）
	if (Target->ActorHasTag(Game::Character::Game_Character_Player.GetTag().GetTagName()))
	{
		return true;
	}

	// 方法2：类型检查
	if (Target->IsA(Source->GetClass()))
	{
		return true;
	}

	// 方法3：控制器检查
	APawn *TargetPawn = Cast<APawn>(Target);
	APawn *SourcePawn = Cast<APawn>(Source);
	if (TargetPawn && SourcePawn)
	{
		// 同一个控制器 = 友方
		if (TargetPawn->GetController() == SourcePawn->GetController())
		{
			return true;
		}
	}

	// 默认为敌方
	return false;
}

void USkillFragment_ExplosionEffect::ApplyGameplayEffectToTarget(
	UPlayerBaseData *Data,
	AActor *SourceActor,
	AActor *TargetActor,
	TSubclassOf<UGameplayEffect> GEClass,
	const FNodeBuffValueFinal &Type)
{
	if (!Data || !Data->ASC || !SourceActor || !TargetActor || !GEClass)
	{
		return;
	}

	// 获取目标的 ASC
	UAbilitySystemComponent *TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)
	{
		return;
	}

	// 1. 创建 GameplayEffect Context
	FGameplayEffectContextHandle EffectContext = Data->ASC->MakeEffectContext();
	EffectContext.AddSourceObject(SourceActor);
	EffectContext.AddInstigator(SourceActor, SourceActor);

	// 2. 创建 GameplayEffect Spec
	FGameplayEffectSpecHandle SpecHandle = Data->ASC->MakeOutgoingSpec(
		GEClass,
		1.0f, // Level
		EffectContext);

	if (!SpecHandle.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("USkillFragment_ExplosionEffect: Failed to create GE Spec"));
		return;
	}

	// 使用 SetByCaller 传递 Type 参数给 GE
	SpecHandle.Data->SetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Data.Strength")),
		Type.Strength);

	SpecHandle.Data->SetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Data.Radius")),
		Type.Radius);

	SpecHandle.Data->SetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Data.Accelerate")),
		Type.Accelerate);

	SpecHandle.Data->SetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Data.BuffDuration")),
		Type.BuffDurationTime);

	FActiveGameplayEffectHandle ActiveHandle = Data->ASC->ApplyGameplayEffectSpecToTarget(
		*SpecHandle.Data.Get(),
		TargetASC);

	if (ActiveHandle.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("USkillFragment_ExplosionEffect: Applied GE %s to %s"),
			   *GEClass->GetName(), *TargetActor->GetName());
		UE_LOG(LogTemp, Log, TEXT("  SetByCaller - Strength: %.2f, Radius: %.2f, Accelerate: %.2f, Duration: %.2f"),
			   Type.Strength, Type.Radius, Type.Accelerate, Type.BuffDurationTime);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("USkillFragment_ExplosionEffect: Failed to apply GE to %s"),
			   *TargetActor->GetName());
	}
}
