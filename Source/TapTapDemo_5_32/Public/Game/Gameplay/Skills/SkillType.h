#pragma once

#include "CoreMinimal.h"

#include "Game/Gameplay/Skills/SkillDetailType.h"

#include "SkillType.generated.h"

UENUM()
enum class ESkillNodeType
{
	NullNode = 0, // 空类型
	StartNode, // 起始类型
	GenerateNode, // 生成物类型
	BuffNode, // buff类型
	BranchNode, // 分支类型
	ParamNode, // 参数类型
	LoopStartNode, // 循环起始类型
	LoopEndNode // 循环结束类型
};

UENUM()
enum class ESwitchTargetType
{
	Unchanged = 0,
	Changed
};

USTRUCT()
struct FSkillNodeInfo
{
	GENERATED_BODY()

	// ----- 公有信息 -----
	ESkillNodeType NodeType = ESkillNodeType::NullNode; // 节点类型
	ESwitchTargetType SwitchTargetType = ESwitchTargetType::Unchanged; // 对象是否变化（是否需要回传）
	int32 DelayTime = 0; // 延迟时间
	int32 OutPinCount = 0; // 输出引脚数量
	EStartNodeType StartNodeType;
	EBranchType BranchType;
	EGeneratedType GeneratedType;
	EParamType ParamType;
	EBuffType BuffType;
	// ----- 公有信息 -----

	// ----- 生成物信息 -----
	float Damage = 0; // 伤害
	float Radius = 0; // 半径
	// ----- 生成物信息 -----

	// ----- 增益信息 -----
	float BuffDurationTime = 1.0f; // buff持续时间
	float BuffValue = 1.0f; // buff值
	// ----- 增益信息 -----

	// ----- 分支信息 -----
	float BranchValue = 0.4f; // 分支值
	// ----- 分支信息 -----
	
	// ----- 参数信息 -----
	int32 ParamIntValue; // 整型参数
	float ParamFloatValue; // 浮点型参数
	// ----- 参数信息 -----	

	// ----- 循环信息 -----
	int32 LoopCount = 3; // 循环次数
	// ----- 循环信息 -----
};

USTRUCT()
struct FAccumulativeInfo
{
	GENERATED_BODY()
	
	float AccDamageValue = 0; // 伤害
	float AccEffectiveTimeValue = 0.0f; // 生效时间
	float AccReduceDelayTimeValue = 0; // 延迟时间
	float RadiusValue = 0.0f; // 半径

	static const FAccumulativeInfo ZeroValue;

	FAccumulativeInfo operator+(const FAccumulativeInfo& Other) const
	{
		FAccumulativeInfo Result;
		Result.AccDamageValue = this->AccDamageValue + Other.AccDamageValue;
		Result.AccEffectiveTimeValue = this->AccEffectiveTimeValue + Other.AccEffectiveTimeValue;
		Result.AccReduceDelayTimeValue = this->AccReduceDelayTimeValue + Other.AccReduceDelayTimeValue;
		Result.RadiusValue = this->RadiusValue + Other.RadiusValue;
		return Result;
	}

	FAccumulativeInfo& operator+=(const FAccumulativeInfo& Other)
	{
		this->AccDamageValue += Other.AccDamageValue;
		this->AccEffectiveTimeValue += Other.AccEffectiveTimeValue;
		this->AccReduceDelayTimeValue += Other.AccReduceDelayTimeValue;
		this->RadiusValue += Other.RadiusValue;
		return *this;
	}
};

UENUM()
enum class OnBranchNode // 判断当前节点是否链接在分支节点上
{
	No = 0,
	TrU,
	FaL
};

USTRUCT()
struct FNodeBuffValueFinal // 结算后用于传给组件的buff信息
{
	GENERATED_BODY()

	EBuffType BuffType = EBuffType::None;
	float BuffDurationTime = 1.0f; // buff持续时间
	float Strength = 1.0f; // 伤害提升
	float Radius = 1.0f; // 半径
	float Accelerate = 1.0f; // 加速
};

USTRUCT()
struct FNodeGenerateValueFinal // 结算后用于传给组件的生成物信息
{
	GENERATED_BODY()

	EGeneratedType GeneratedType = EGeneratedType::None; // 生成物类型
	float Damage = 0.0f; // 伤害
	float Radius = 0.0f; // 半径
};

