// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include <random>

#include "Game/Gameplay/Skills/SkillNode.h"
#include "SkillsManagerSubsystem.generated.h"

class USkillNode;
enum class ESkillNodeType;
struct FSkillNodeInfo;

/**
 * 
 */
UCLASS()
class TAPTAPDEMO_5_32_API USkillsManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	USkillNode* NewSkillNode(const FSkillNodeInfo& NodeInfo);
	void DeleteSkillNode(USkillNode* Node);

	bool ConnectNode(USkillNode* ParentNode, USkillNode* ChildNode);
	void DisconnectNode(USkillNode* ParentNode, USkillNode* ChildNode);

	void UpdateLoopEndNodes(TArray<USkillNode*>& LoopEndArray, USkillNode* LoopStartNode);

	USkillNode* GetSkillNodeByHash(const int32 HashID) const;

	UFUNCTION()
	static USkillsManagerSubsystem* Get(const UObject* WorldContextObject);

private:
	UPROPERTY()
	TMap<int32, USkillNode*> HashToNode;
	
	std::mt19937 HashGenerateEngine;

	TArray<USkillNode*>* TemporaryLoopEndNodeArray;
	TSet<int32> TemporaryLoopStartNodeBehindTarget;
	void DfsStuffEndNodes(USkillNode* CurrentNode);

	/**
	 * 重新计算当前节点的所有祖先节点的循环终点节点（不计算当前节点）
	 */
	void FlushParentLoopStartNode(USkillNode* _Node);

	/**
	 * 重新计算当前节点的祖先节点的循环终点节点（包含当前节点）
	 */
	void FlushParentLoopStartNodeWithSelf(USkillNode* Node);
};
