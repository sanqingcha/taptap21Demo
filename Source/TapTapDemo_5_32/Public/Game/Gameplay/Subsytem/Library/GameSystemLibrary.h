// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameSystemLibrary.generated.h"

class UGameplayEffect;
struct FGameplayAttribute;
/**
 * 
 */
UCLASS()
class TAPTAPDEMO_5_32_API UGameSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	public:
	UFUNCTION(BlueprintCallable,Category="GameLibrary" ,meta = (level = "1",AdvancedDisplay = "level" , Keywords = "Attack"))
	static bool ApplyEffectToActor(AActor* Source , AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass , float level);

	UFUNCTION(BlueprintCallable,Category="GameLibrary" ,meta = (Keywords = "GetActor"))
	static void Game_GetAllActorsOfClass(UObject* WorldContext, TSubclassOf<AActor> ActorClass, TArray<AActor*>& OutActors);
};
