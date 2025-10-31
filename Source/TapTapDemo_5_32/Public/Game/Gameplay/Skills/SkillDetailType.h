﻿#pragma once

#include "CoreMinimal.h"
#include "SkillDetailType.generated.h"


// ------ 各类型节点的具体种类 ------ 

UENUM(BlueprintType)
enum class EStartNodeType // 起始节点具体类型
{
	None = 0,
	LeftButton,
	OnHit,
	OnJump,
	EnemyDied,
	RightButton
};

UENUM(BlueprintType)
enum class EBranchType // 分支具体类型
{
	None = 0,
	BloodLow,
	InAir
};

UENUM(BlueprintType)
enum class EGeneratedType // 生成物具体类型
{
	None = 0, // 空类型
	StandardBullet, // 标准子弹
	EnhancedBullet, // 强化子弹
	Explosion // 爆炸
};

UENUM(BlueprintType)
enum class EParamType :uint8// 参数具体类型
{
	None = 0,
	Damage,
	EffectiveTime,
	ReduceDelayTime,
	Radius
};

UENUM(BlueprintType)
enum class EBuffType // 增益具体类型
{
	None = 0,
	Accelerate,
	Strength,
	Size,
	AttackSpeed
};

