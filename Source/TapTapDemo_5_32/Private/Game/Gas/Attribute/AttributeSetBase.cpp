// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gas/Attribute/AttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

void UPlayerAttribute::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute==GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		UE_LOG(LogTemp, Error, TEXT("Player Health Changed Once"));
	}
	if (Data.EvaluatedData.Attribute==GetMaxHealthAttribute())
	{
		OnHPPercentChangeDelegate.Broadcast(GetHPPercent());
	}
}

const float UPlayerAttribute::GetHPPercent()
{
	return FMath::Clamp(GetHealth()/GetMaxHealth(),0.f,1.f);
}

void UEnemyAttribute::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute==GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		UE_LOG(LogTemp, Error, TEXT("Player Health Changed Once"));
	}
}
