// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameCharacterBase.h"
#include "Game/Gameplay/Interface/PlayerPawnInterface.h"
#include "PlayerCharacter.generated.h"

class UDialoageComponent;
class UCameraComponent;
class UAbilitySystemComponent;
class UPlayerAttribute;

UCLASS()
class TAPTAPDEMO_5_32_API APlayerCharacter : public AGameCharacterBase ,public IPlayerPawnInterface
{
	GENERATED_BODY()
public:
	APlayerCharacter();
	virtual void BeginPlay() override;
	virtual USpringArmComponent* GetSpringArmComponent_Implementation() override;
	virtual void CallJump_Implementation() override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override{return PlayerAbilitySysComp;};
	virtual void Die_Implementation() override{};


	
	/**Dialoage*/
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> DialoageWidgetRoot;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UDialoageComponent> DialoageComp;
	/**end*/
	
	/**Gas*/
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> PlayerAbilitySysComp;
	/**end*/

	/**Camera*/
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<USpringArmComponent>SpringArmComp;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UCameraComponent> CameraComp;
	/**end*/
};
