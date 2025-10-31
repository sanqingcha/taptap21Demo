// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Game/Gameplay/Interface/CharacterBaseInterface.h"
#include "GameFramework/Character.h"
#include "GameCharacterBase.generated.h"


class UExecuteSkillComponent;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class TAPTAPDEMO_5_32_API AGameCharacterBase : public ACharacter ,public IAbilitySystemInterface,public ICharacterBaseInterface
{
	GENERATED_BODY() 

public:
	AGameCharacterBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Die_Implementation() override{};
	
	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override{check(0) ;return nullptr;};
	FORCEINLINE virtual UCharacterMovementComponent* GetCharacterMovementComp() override;

	virtual void BindAttributeDelegate(){check(0)};

	/**Skill*/
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UExecuteSkillComponent> ExecuteComp;
	/**end*/
};
