// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AttributeSetBase.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/**
 * 
 */


UCLASS()
class TAPTAPDEMO_5_32_API UAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()
};

UCLASS()
class TAPTAPDEMO_5_32_API UPlayerAttribute : public UAttributeSet
{
	GENERATED_BODY()
	public:
	UPlayerAttribute(){};
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "PlayerAttribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UPlayerAttribute, Health);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "PlayerAttribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UPlayerAttribute, MaxHealth);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "PlayerAttribute")
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UPlayerAttribute, Level);
	
};

UCLASS()
class TAPTAPDEMO_5_32_API  UEnemyAttribute: public UAttributeSet
{
	GENERATED_BODY()
public:
	UEnemyAttribute(){};
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "EnemyAttribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UEnemyAttribute, Health);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "EnemyAttribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UEnemyAttribute, MaxHealth);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "EnemyAttribute")
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UEnemyAttribute, Level);

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "EnemyAttribute")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UEnemyAttribute, AttackPower);
	
    UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "EnemyAttribute")
	FGameplayAttributeData MaxSpeed;
	ATTRIBUTE_ACCESSORS(UEnemyAttribute, MaxSpeed);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "EnemyAttribute")
	FGameplayAttributeData AttackCoolDown ;
	ATTRIBUTE_ACCESSORS(UEnemyAttribute, AttackCoolDown);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "EnemyAttribute")
	FGameplayAttributeData ProjectileMaxSpeed;
	ATTRIBUTE_ACCESSORS(UEnemyAttribute, ProjectileMaxSpeed);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "EnemyAttribute")
	FGameplayAttributeData JumpMaxHigh;
	ATTRIBUTE_ACCESSORS(UEnemyAttribute, JumpMaxHigh);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "EnemyAttribute")
	FGameplayAttributeData FlyMaxHigh;
	ATTRIBUTE_ACCESSORS(UEnemyAttribute, FlyMaxHigh);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "EnemyAttribute")
	FGameplayAttributeData AttackRandiu;
	ATTRIBUTE_ACCESSORS(UEnemyAttribute, AttackRandiu);

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "EnemyAttribute")
	FGameplayAttributeData CheckRandiu;
	ATTRIBUTE_ACCESSORS(UEnemyAttribute, CheckRandiu);
};