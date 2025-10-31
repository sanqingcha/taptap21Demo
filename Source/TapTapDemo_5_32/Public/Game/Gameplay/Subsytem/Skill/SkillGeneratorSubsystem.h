// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/Gameplay/Skills/SkillType.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SkillGeneratorSubsystem.generated.h"

class ASkillNodeActor;
class USkillsManagerSubsystem;
class USkillsRuntimeSubsystem;
class USkillNodeAsset;
class USkillNode;
enum class ESkillNodeType;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FNodeSaveData
{
	GENERATED_BODY()
	FNodeSaveData() = default;
	FNodeSaveData(USkillNode* inNode,ESkillNodeType inType):Node(inNode),Type(inType){}
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	ESkillNodeType Type;

	UPROPERTY();
	USkillNode* Node = nullptr;
};


UCLASS()
class TAPTAPDEMO_5_32_API USkillGeneratorSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	public:
	UFUNCTION(BlueprintCallable)
	void Resister(USkillNodeAsset* InitData,TSubclassOf<ASkillNodeActor> inNodeActorCLass,TArray<TEnumAsByte<EObjectTypeQuery> > inCheckObjectTypes);
	UFUNCTION(BlueprintCallable)
	void RegisterCenterCheckPos(const FVector inCenterPos){CenterPos = inCenterPos;};
	//void Resister(USkillNode* InitData);
	//UFUNCTION(BlueprintCallable,meta = (AdvancedDisplay = "StartType"))


	UFUNCTION(BlueprintCallable)
	void AddOneNodeActorInBuildSpace(TEnumAsByte<ESkillNodeType> Type);
	UFUNCTION(BlueprintCallable)
	bool GenerateSkills(TEnumAsByte<ESkillNodeType> Type,TEnumAsByte<EStartNodeType> StartType,FNodeSaveData& NodeData);
protected:
	bool CheckPosBlock(const FVector& Pos);
	void SpawnNode(const FVector& Pos,FNodeSaveData& Data);
private:
	UPROPERTY()
	TArray<TEnumAsByte<EObjectTypeQuery> > CheckObjectTypes;
	UPROPERTY()
	TSubclassOf<ASkillNodeActor> NodeActorCLass;
	FVector CenterPos;
	USkillsManagerSubsystem* SkillManager;
	UPROPERTY()
	TObjectPtr<USkillNodeAsset> SkillAsset;
	UPROPERTY()
	TMap<uint32,FNodeSaveData> SavedData;
};
