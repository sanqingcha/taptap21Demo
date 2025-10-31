// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameSystemLibrary.generated.h"

class AEnemyCharacter;
class USkillGeneratorSubsystem;
enum class EBuffType;
enum class EParamType : uint8;
enum class EGeneratedType;
enum class EBranchType;
enum class EStartNodeType;
enum class ESwitchTargetType;
enum class ESkillNodeType;
struct FSkillNodeInfo;
struct FNodeSaveData;
class UPlayerBaseData;
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

	UFUNCTION(BlueprintCallable,Category="GameLibrary" ,meta = (Keywords = "GetActor|TrriggerLibrary"))
	static void Game_GetAllActorsOfClass(UObject* WorldContext, TSubclassOf<AActor> ActorClass, TArray<AActor*>& OutActors);
	UFUNCTION(BlueprintCallable,Category="GameLibrary" ,meta = (Keywords = "GetActor|TrriggerLibrary"))
	static AActor* SpawnActorByClass(UObject* Context, UClass* InClass , const FVector Location, const FRotator Rotation);
	UFUNCTION(BlueprintPure,Category="GameLibrary" ,meta = (Keywords = "GetActor|TrriggerLibrary"))
	static USkillGeneratorSubsystem* GetSkillGenerateNodeSystem(UObject* Context);

	UFUNCTION(BlueprintPure,Category="GameLibrary" ,meta = (Keywords = "Player"))
	static UPlayerBaseData* GetPlayerData();

	UFUNCTION(BlueprintPure,Category="GameLibrary" ,meta = (Keywords = "Cast"))
	static const FString MakeDynamicString(FString SourceString,const TArray<int32> Values);

	UFUNCTION(BlueprintCallable,Category="GameLibrary" ,meta = (Keywords = "PlayerController"))
	static void SetCanPlayerMove(UPlayerBaseData* Data,bool CanMove);


	UFUNCTION(BlueprintCallable,Category="GameLibrary" ,meta = (Keywords = "SkillNode"))
	static const FSkillNodeInfo& GetNodeData(const FNodeSaveData& Data);

	UFUNCTION(BlueprintPure,Category="GameLibrary" ,meta = (Keywords = "SkillNode"))
	static TEnumAsByte<ESkillNodeType> GetSkillNodeType(const FSkillNodeInfo& Data);
	UFUNCTION(BlueprintPure,Category="GameLibrary" ,meta = (Keywords = "SkillNode"))
	static TEnumAsByte<ESwitchTargetType> GetSwitchTargetType(const FSkillNodeInfo& Data);
	UFUNCTION(BlueprintPure,Category="GameLibrary" ,meta = (Keywords = "SkillNode"))
	static TEnumAsByte<EStartNodeType> GetStartNodeType(const FSkillNodeInfo& Data);
	UFUNCTION(BlueprintPure,Category="GameLibrary" ,meta = (Keywords = "SkillNode"))
	static TEnumAsByte<EBranchType> GetBranchType(const FSkillNodeInfo& Data);
	UFUNCTION(BlueprintPure,Category="GameLibrary" ,meta = (Keywords = "SkillNode"))
	static TEnumAsByte<EGeneratedType> GetGeneratedType(const FSkillNodeInfo& Data);
	UFUNCTION(BlueprintPure,Category="GameLibrary" ,meta = (Keywords = "SkillNode"))
	static TEnumAsByte<EParamType > GetParamType (const FSkillNodeInfo& Data);
	UFUNCTION(BlueprintPure,Category="GameLibrary" ,meta = (Keywords = "SkillNode"))
	static TEnumAsByte<EBuffType> GetBuffType(const FSkillNodeInfo& Data);
};
