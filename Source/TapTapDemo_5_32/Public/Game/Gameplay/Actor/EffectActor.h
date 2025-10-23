// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class TAPTAPDEMO_5_32_API AEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AEffectActor();
	UFUNCTION(BlueprintCallable, Category = "EffectActor")
	inline void ResetLevel(float Newlevel){ActorLevel = Newlevel;}
	UFUNCTION(BlueprintCallable, Category = "EffectActor")
	void ApplyEffectToTarget(AActor* TargetActor);

protected:
	float ActorLevel = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
};
