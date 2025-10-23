// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCharacterBase.h"
#include "EnemyCharacter.generated.h"

class UWidgetComponent;
class UEnemyAttribute;

UCLASS()
class TAPTAPDEMO_5_32_API AEnemyCharacter : public AGameCharacterBase
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override{return EnemyAbilitySysComp;};
	virtual void Die_Implementation() override{};
	
protected:
	/**用于显示敌人的血条蓝条等信息*/
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> EnemyDataWidget;
	
	/**Gas*/
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> EnemyAbilitySysComp;
	/**end*/
};
