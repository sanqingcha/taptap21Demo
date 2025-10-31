// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Game/Gameplay/Skills/SkillType.h"

#include "SkillNode.generated.h"


class USkillsManagerSubsystem;
class UExecuteSkillComponent;
class AActor;


/**
 * 
 */
UCLASS()
class TAPTAPDEMO_5_32_API USkillNode : public UObject
{
	GENERATED_BODY()

public:
	static const FName Tag_SkillComponent;
	
	void Initialize(const int32 _HashID, const FSkillNodeInfo& _NodeInfo);

	void SetDefaultValue();
	
	bool AddChildNode(USkillNode* Node, OnBranchNode Branch = OnBranchNode::No);
	void RemoveChildNode(USkillNode* Node, OnBranchNode Branch = OnBranchNode::No);

	/**
	 * 触发后续节点
	 */
	void Delivery(const AActor* Target);

	virtual void Trigger(const AActor* Target);
	virtual void Ability(const AActor* Target) { }
	virtual bool Branch(const AActor* Target);
	virtual void Start() { }

	template<typename Func>
	void ForEachChild(Func&& TarFunc) const
	{
		for (USkillNode* Child : ChildNodes)
		{
			TarFunc(Child);
		}
	}

	int32 CalculateDelayTime();

#pragma region UTIL
	
	bool HasChild() const;
	
	void SetParentNode(USkillNode* Node);
	USkillNode* GetParentNode() const;

	int32 GetHashID() const;
	int32 GetDelayTime() const;

	ESkillNodeType GetNodeType() const;

	USkillNode* GetLoopStartNode() const;
	void SetLoopStartNode(USkillNode* Node);

	void ForceRemoveAllChild();

	USkillNode* GetFirstChildNode();

	FAccumulativeInfo GetAccumulativeInfo() const;
	void SetAccumulativeInfo(const FAccumulativeInfo& Info);

	TArray<USkillNode*>& GetLoopEndNodesRef();

	void SetBranchTrueNode(USkillNode* Node);
	void SetBranchFalseNode(USkillNode* Node);

	USkillNode* GetBranchTrueNode();
	USkillNode* GetBranchFalseNode();

	const FSkillNodeInfo& GetNodeInfo() const;

#pragma endregion

protected:
	UPROPERTY()
	TArray<USkillNode*> ChildNodes;

	UPROPERTY()
	TArray<USkillNode*> LoopEndNodes;

	UPROPERTY()
	USkillNode* LoopStartNode = nullptr;

	UPROPERTY()
	USkillNode* ParentNode = nullptr;

	UPROPERTY()
	int32 HashID;

	UPROPERTY()
	FSkillNodeInfo NodeInfo;

	UPROPERTY()
	FAccumulativeInfo AccumulativeInfo;

	UPROPERTY()
	USkillNode* BranchTrueNode = nullptr;

	UPROPERTY()
	USkillNode* BranchFalseNode = nullptr;
	
	int32 AllDelayTime = 0;

	UExecuteSkillComponent* GetTargetExecuteSkillComponent(const AActor* Target) const;

	UPROPERTY()
	USkillsManagerSubsystem* SkillsManagerSubsystem = nullptr;
};
