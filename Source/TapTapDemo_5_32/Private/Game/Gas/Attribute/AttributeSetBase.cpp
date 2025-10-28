// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gas/Attribute/AttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "Game/Gameplay/Interface/CharacterBaseInterface.h"
#include "Game/Gameplay/Interface/PlayerPawnInterface.h"
#include "Game/Gameplay/Player/SuperPlayerController.h"
#include "Game/Gameplay/Player/Data/PlayerBaseData.h"
#include "Net/UnrealNetwork.h"

void UPlayerAttribute::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute==GetHealthAttribute())
	{
		if (GetHealth()<=0)
		{
			ICharacterBaseInterface::Execute_Die(ASuperPlayerController::PlayerData_Static->PC->GetPawn());
		}
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		//UE_LOG(LogTemp, Error, TEXT("Player Health Changed Once"));
		OnHPPercentChangeDelegate.Broadcast(GetHPPercent());
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
		//UE_LOG(LogTemp, Error, TEXT("Player Health Changed Once"));
	}
}
