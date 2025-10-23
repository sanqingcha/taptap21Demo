// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillNode.generated.h"


UENUM()
enum class ESkillNodeType
{
	NullNode = 0, // 空类型
	StartNode, // 起始类型
	GenerateNode, // 生成物类型
	BuffNode, // buff类型
	BranchNode, // 分支类型
	LoopStartNode, // 循环起始类型
	LoopEndNode // 循环结束类型
};

UENUM()
enum class EGeneratedType
{
	None = 0, // 空类型
	StandardBullet, // 标准子弹
	EnhancedBullet, // 强化子弹
	Explosion // 爆炸
};


USTRUCT()
struct FSkillNodeInfo
{
	GENERATED_BODY()

	ESkillNodeType NodeType = ESkillNodeType::NullNode; // 节点类型
	int32 DelayTime = 0; // 延迟时间
	int32 OutPinCount = 0; // 输出引脚数量

	int32 Damage = 0; // 伤害
	int32 Radius = 0; // 半径
	int32 Range = 0; // 射程

	EGeneratedType GeneratedType = EGeneratedType::None; // 生成物

	float BuffDurationTime = 1.0f; // buff持续时间
	int32 BuffValue = 1; // buff值

	float BranchValue = 0.4f; // 分支值

	int32 ParamIntValue = 0; // 整型参数
	float ParamFloatValue = 0.0f; // 浮点型参数
	
	int32 LoopCount = 3; // 循环次数
	
};


/**
 * 
 */
UCLASS()
class TAPTAPDEMO_5_32_API USkillNode : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(const int32 _HashID, const FSkillNodeInfo& _NodeInfo);

	void SetDefaultValue();
	
	void AddChildNode(USkillNode* Node);
	void RemoveChildNode(USkillNode* Node);

	virtual void Trigger(const UObject* Target);
	virtual void Ability() { }
	virtual bool Branch();

	template<typename Func>
	void ForEachChild(Func&& TarFunc) const
	{
		for (USkillNode* Child : ChildNodes)
		{
			TarFunc(Child);
		}
	}

	bool HasChild() const;

#pragma region UTIL
	
	void SetParentNode(USkillNode* Node);
	USkillNode* GetParentNode() const;

	int32 GetHashID() const;
	int32 GetDelayTime() const;

	ESkillNodeType GetNodeType() const;

	USkillNode* GetLoopStartNode() const;
	void SetLoopStartNode(USkillNode* Node);

	void ForceRemoveAllChild();

	USkillNode* GetFirstChildNode();

	TArray<USkillNode*>& GetLoopEndNodesRef();

#pragma endregion

private:
	UPROPERTY()
	TArray<USkillNode*> ChildNodes;

	UPROPERTY()
	TArray<USkillNode*> LoopEndNodes;

	UPROPERTY()
	USkillNode* LoopStartNode = nullptr;

	UPROPERTY()
	USkillNode* ParentNode;

	UPROPERTY()
	int32 HashID;

	UPROPERTY()
	FSkillNodeInfo NodeInfo;
	
	int32 AllDelayTime = 0;
};
