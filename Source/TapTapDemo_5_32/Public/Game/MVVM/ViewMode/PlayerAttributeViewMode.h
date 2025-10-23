// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMGViewModelBase.h"
#include "Game/Gameplay/Player/Data/PlayerBaseData.h"
#include "PlayerAttributeViewMode.generated.h"

/**
 * 
 */

UCLASS()
class TAPTAPDEMO_5_32_API UPlayerAttributeViewMode : public UUMGViewModelBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,FieldNotify,Setter,Getter)
	float CurrHealth_UI = 100.f;
	UPROPERTY(BlueprintReadWrite,FieldNotify,Setter,Getter)
	float MaxHealth_UI = 100.f;
	UPROPERTY(BlueprintReadWrite,FieldNotify,Setter,Getter)
	float HealthPercent = 1.f;
	
	virtual void InitialViewModel(UPlayerBaseData* BaseData)override;
	
private: 
	void  SetCurrHealth_UI(float Value);
	float GetCurrHealth_UI()const{return CurrHealth_UI;};
	void  SetMaxHealth_UI(float Value);
	float GetMaxHealth_UI()const{return MaxHealth_UI;};
	void  SetHealthPercent(float Value);
	float GetHealthPercent()const{return HealthPercent;};

};
