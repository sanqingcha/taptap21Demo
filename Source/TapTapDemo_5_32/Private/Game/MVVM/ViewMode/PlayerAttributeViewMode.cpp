// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MVVM/ViewMode/PlayerAttributeViewMode.h"

#include "AbilitySystemComponent.h"
#include "Game/Gameplay/Player/Data/PlayerBaseData.h"
#include "Game/Gas/Attribute/AttributeSetBase.h"


void UPlayerAttributeViewMode::InitialViewModel(UPlayerBaseData* inBaseData)
{
	Super::InitialViewModel(inBaseData);
	check(BaseData);

	const UPlayerAttribute* PlayerAS = Cast<UPlayerAttribute>(BaseData->ASC->GetAttributeSet(UAttributeSet::StaticClass()));

	
	SetCurrHealth_UI(PlayerAS->GetHealth());
	BaseData->ASC->GetGameplayAttributeValueChangeDelegate(PlayerAS->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData &Data)
	{
		SetCurrHealth_UI(Data.NewValue);
	});
	
	SetMaxHealth_UI(PlayerAS->GetMaxHealth());
	BaseData->ASC->GetGameplayAttributeValueChangeDelegate(PlayerAS->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData &Data)
	{
		SetMaxHealth_UI(Data.NewValue);
	});
}

void UPlayerAttributeViewMode::SetCurrHealth_UI(float Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(CurrHealth_UI,Value);
	SetHealthPercent(FMath::Clamp(CurrHealth_UI/MaxHealth_UI,0.f,1.f));
}

void UPlayerAttributeViewMode::SetMaxHealth_UI(float Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(MaxHealth_UI,Value);
	SetHealthPercent(FMath::Clamp(CurrHealth_UI/MaxHealth_UI,0.f,1.f));
}

void UPlayerAttributeViewMode::SetHealthPercent(float Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(HealthPercent,Value);
	
}

