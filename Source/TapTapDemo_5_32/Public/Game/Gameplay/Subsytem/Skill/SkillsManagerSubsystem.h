// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include <random>

#include "Game/Gameplay/Skills/SkillNode.h"
#include "SkillsManagerSubsystem.generated.h"


class USkillsRuntimeSubsystem;
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
	/**
	 * 新建技能节点
	 * @param NodeInfo 技能节点信息
	 * @return 新建的技能节点
	 */
	USkillNode* NewSkillNode(const FSkillNodeInfo& NodeInfo, TSubclassOf<USkillNode> SkillNodeClass);

	/**
	 * 删除技能节点
	 * @param Node 待删除的技能节点
	 * @param Branch 是否连接在父节点的分支上
	 */
	void DeleteSkillNode(USkillNode* Node, OnBranchNode Branch = OnBranchNode::No);

	/**
	 * 连接两个技能节点
	 * @param ParentNode 父节点
	 * @param ChildNode 子节点
	 * @param Branch 子节点是否连接在父节点的分支上
	 * @return 是否成功连接
	 */
	bool ConnectNode(USkillNode* ParentNode, USkillNode* ChildNode, OnBranchNode Branch = OnBranchNode::No);

	/**
	 * 断开两个技能节点
	 * @param ParentNode 父节点
	 * @param ChildNode 子节点
	 * @param Branch 子节点是否连接在父节点的分支上
	 */
	void DisconnectNode(USkillNode* ParentNode, USkillNode* ChildNode, OnBranchNode Branch = OnBranchNode::No);

	void UpdateLoopEndNodes(TArray<USkillNode*>& LoopEndArray, USkillNode* LoopStartNode);
	USkillNode* GetSkillNodeByHash(const int32 HashID) const;

	UFUNCTION()
	static USkillsManagerSubsystem* Get(const UObject* WorldContextObject);

	// 攻击参数在结算时的修正参数，运算为相乘运算
	static const float AdditionalParamForAttack;

	void SetCanRun(bool Value);
	bool GetCanRun();
	
private:
	UPROPERTY()
	TMap<int32, USkillNode*> HashToNode;

	UPROPERTY()
	USkillsRuntimeSubsystem* SkillsRuntimeSubsystem;
	
	std::mt19937 HashGenerateEngine;

	bool bCanRun = false;

	TArray<USkillNode*>* TemporaryLoopEndNodeArray;
	TSet<int32> TemporaryLoopStartNodeBehindTarget;
	void DfsStuffEndNodes(USkillNode* CurrentNode);

	USkillsRuntimeSubsystem* GetSkillsRuntimeSubsystem();

	/**
	 * 重新计算当前节点的所有祖先节点的循环终点节点（不计算当前节点）
	 */
	void FlushParentLoopStartNode(USkillNode* _Node);

	/**
	 * 重新计算当前节点的祖先节点的循环终点节点（包含当前节点）
	 */
	void FlushParentLoopStartNodeWithSelf(USkillNode* _Node);

	/**
	 * 递归向下更新当前节点的累加参数，然后向上更新延迟
	 */
	void DfsUpdateForwardParam(USkillNode* _Node);

	/**
	 * 向上更新节点的延迟时间至根
	 */
	void UpdateTimeDelay(USkillNode* _UpdateStartNode);
};
