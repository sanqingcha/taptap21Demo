// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Game/Gameplay/Skills/SkillType.h"
#include "SkillNodeAsset.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FStartNodeInitData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="StartNode")
	int32 OutPinCount = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="StartNode")
	float Delay = 0;
};

USTRUCT(BlueprintType)
struct FSkillNodeTemplateData
{
	GENERATED_BODY()
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DefaultDescription  ="这是一个稀有节点";
	/**0 ：节点伤害
	 * 1 ：
	 #1#
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FunctionDescription = "会让伤害增加{0}";*/

	/**Default*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Default")
	float MinDelayTime = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Default")
	float MaxDelayTime = 100.0f;

	/**BufferNode*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="BufferNode")
	float MaxBuffDurationTime = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="BufferNode")
	float MinBuffDurationTime = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="BufferNode")
	float MaxBufferValue = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="BufferNode")
	float MinBufferValue = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="BufferNode")
	int32 Buffer_OutPinCount = 1;

	/**GenerateNode*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="GenerateNode")
	float MinDamage = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="GenerateNode")
	float MaxDamage = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="GenerateNode")
	int32 Genterate_OutPinCount = 1;

	/**ParamsNode*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="ParamsNode")
	int32 MinParamIntValue = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="ParamsNode")
	int32 MaxParamIntValue = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="ParamsNode")
	float MinParamFloatValue = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="ParamsNode")
	float MaxParamFloatValue = 1.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="GenerateNode")
	int32 Params_OutPinCount = 1;

	/**DelayNode*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="LoopNode")
	int32 Loop_OutPinCount = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="LoopNode")
	int32 LoopCount = 3;
	
	/**StartNode*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="StartNode")
	float Delay_StartNode = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="StartNode")
	TMap<EStartNodeType,FStartNodeInitData> StartNodeOutPinCount;
};


UCLASS()
class TAPTAPDEMO_5_32_API USkillNodeAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,Blueprintable)
	FSkillNodeTemplateData Data;
};
