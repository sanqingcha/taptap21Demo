// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/Gas/Attribute/AttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "Game/Gameplay/Interface/CharacterBaseInterface.h"
#include "Game/Gameplay/Interface/PlayerPawnInterface.h"
#include "Game/Gameplay/Player/SuperPlayerController.h"
#include "Game/Gameplay/Player/Data/PlayerBaseData.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraShakeBase.h"
#include "Kismet/GameplayStatics.h"

void UPlayerAttribute::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data)
{
	Super::PostGameplayEffectExecute(Data);
	// 安全地获取属性名称
	FString AttributeName = TEXT("Unknown");
	if (Data.EvaluatedData.Attribute.IsValid())
	{
		AttributeName = Data.EvaluatedData.Attribute.GetName();
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		float NewHealth = GetHealth();

		// 检测受击（生命值下降）
		if (PreviousHealth > 0.0f && NewHealth < PreviousHealth)
		{
			float DamageTaken = PreviousHealth - NewHealth;
			TriggerHitCameraShake(Data);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No damage detected - PreviousHealth: %.2f, NewHealth: %.2f (Maybe healing or first init?)"),
				   PreviousHealth, NewHealth);
		}

		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
		PreviousHealth = GetHealth();
		OnHPPercentChangeDelegate.Broadcast(GetHPPercent());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Not Health attribute, skipping damage detection"));
	}

	if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		// 初始化 PreviousHealth
		if (PreviousHealth < 0.0f)
		{
			PreviousHealth = GetHealth();
			UE_LOG(LogTemp, Error, TEXT(">>> PreviousHealth initialized to: %.2f"), PreviousHealth);
		}
		OnHPPercentChangeDelegate.Broadcast(GetHPPercent());
	}
}

const float UPlayerAttribute::GetHPPercent()
{
	return FMath::Clamp(GetHealth() / GetMaxHealth(), 0.f, 1.f);
}

void UPlayerAttribute::TriggerHitCameraShake(const struct FGameplayEffectModCallbackData &Data)
{

	// 检查是否配置了相机震动类
	if (!HitCameraShakeClass)
	{
		UE_LOG(LogTemp, Error, TEXT("[FAILED] HitCameraShakeClass not set in PlayerAttribute! Please configure it in Blueprint."));
		return;
	}

	// 获取目标 Actor（玩家角色）
	AActor *TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
	if (!TargetActor)
	{
		UE_LOG(LogTemp, Error, TEXT("[FAILED] TargetActor is null in TriggerHitCameraShake"));
		return;
	}

	// 获取玩家控制器
	APlayerController *PC = Cast<APlayerController>(TargetActor->GetInstigatorController());
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("[TRY] GetInstigatorController failed, trying GetPlayerController..."));
		// 尝试通过另一种方式获取
		PC = UGameplayStatics::GetPlayerController(TargetActor->GetWorld(), 0);
	}

	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("[FAILED] PlayerController is null"));
		return;
	}

	if (!PC->PlayerCameraManager)
	{
		UE_LOG(LogTemp, Error, TEXT("[FAILED] PlayerCameraManager is null"));
		return;
	}

	UCameraShakeBase *ShakeInstance = PC->PlayerCameraManager->StartCameraShake(HitCameraShakeClass, HitShakeScale);
}

void UEnemyAttribute::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
}
