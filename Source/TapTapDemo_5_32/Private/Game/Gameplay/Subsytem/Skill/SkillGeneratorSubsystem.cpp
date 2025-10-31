// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Subsytem/Skill/SkillGeneratorSubsystem.h"

#include "Game/Gameplay/Actor/SkillActor/SkillNodeActor.h"
#include "Game/Gameplay/Skills/SkillChildType.h"
#include "Game/Gameplay/Skills/SkillType.h"
#include "Game/Gameplay/Skills/ChildNodes/BranchNode.h"
#include "Game/Gameplay/Skills/ChildNodes/BuffNode.h"
#include "Game/Gameplay/Skills/ChildNodes/GenerateNode.h"
#include "Game/Gameplay/Skills/ChildNodes/LoopNode.h"
#include "Game/Gameplay/Skills/ChildNodes/ParamNode.h"
#include "Game/Gameplay/Skills/ChildNodes/StartNode.h"
#include "Game/Gameplay/Subsytem/Skill/SkillsManagerSubsystem.h"
#include "Game/Gameplay/Subsytem/Skill/Data/SkillNodeAsset.h"
#include "Kismet/KismetSystemLibrary.h"

void USkillGeneratorSubsystem::Resister(USkillNodeAsset* InitData,
                                        TSubclassOf<ASkillNodeActor> inNodeActorCLass,
                                        TArray<TEnumAsByte<EObjectTypeQuery> > inCheckObjectTypes)
{
	SkillAsset = InitData;
	SkillManager =  GetGameInstance()->GetSubsystem<USkillsManagerSubsystem>();
	NodeActorCLass = inNodeActorCLass;
	CheckObjectTypes = inCheckObjectTypes;
}

bool USkillGeneratorSubsystem::GenerateSkills(TEnumAsByte<ESkillNodeType> Type,TEnumAsByte<EStartNodeType> StartType,FNodeSaveData& NodeData)
{
	if (!ensure(SkillAsset))return false;
	if (!ensure(SkillManager))return false;
	FSkillNodeTemplateData& Data = SkillAsset->Data;

	//UE_LOG(LogTemp,Error,TEXT("USkillGeneratorSubsystem::GenerateSkills::type = %s"),*UEnum::GetValueAsString(Type));
	UKismetSystemLibrary::PrintString(GetWorld(),FString::Printf(TEXT("USkillGeneratorSubsystem::GenerateSkills::type = %s"),*UEnum::GetValueAsString(Type)));
	
	switch (Type)
	{
	case ESkillNodeType::BranchNode:
		{
			FBranchNodeInfo BranchNode = FBranchNodeInfo(
			(EBranchType)FMath::RandRange(1,2),
				FMath::FRandRange(Data.MinDelayTime,Data.MaxDelayTime)
				);
			USkillNode* Node =  SkillManager->NewSkillNode(BranchNode,UBranchNode::StaticClass());
			NodeData = SavedData.Emplace(GetTypeHash(Node),FNodeSaveData(Node,ESkillNodeType::BranchNode));
			return true;
		}
	case ESkillNodeType::GenerateNode:
		{
			FGenerateNodeInfo GenerateNode = FGenerateNodeInfo(
			(EGeneratedType)FMath::RandRange(1,3),
				FMath::FRandRange(Data.MinDamage,Data.MaxDamage),
				FMath::FRandRange(Data.MinDelayTime,Data.MaxDelayTime),
				Data.Genterate_OutPinCount
				);
			USkillNode* Node =  SkillManager->NewSkillNode(GenerateNode,UGenerateNode::StaticClass());
			NodeData = SavedData.Emplace(GetTypeHash(Node),FNodeSaveData(Node,ESkillNodeType::GenerateNode));
			return true;
		}
	case ESkillNodeType::BuffNode:
		{
			//FBuffNodeInfo(EBuffType _BuffType, float _BuffDurationTime, float _BuffValue, float _DelayTime = 100.0f, int32 _OutPinCount = 1);
			FBuffNodeInfo BuffNode = FBuffNodeInfo(
				(EBuffType)FMath::RandRange(1,4),
				FMath::FRandRange(Data.MinBuffDurationTime,Data.MaxBuffDurationTime),
				FMath::FRandRange(Data.MinBufferValue,Data.MaxBufferValue),
				FMath::FRandRange(Data.MinDelayTime,Data.MaxDelayTime),
				Data.Buffer_OutPinCount
					);
			USkillNode* Node =  SkillManager->NewSkillNode(BuffNode,UBuffNode::StaticClass());
			NodeData = SavedData.Emplace(GetTypeHash(Node),FNodeSaveData(Node,ESkillNodeType::BuffNode));
			return true;
		}
	case ESkillNodeType::LoopStartNode:
		{
			//FLoopNodeInfo(float _DelayTime, int32 _OutPinCount = 1, int32 _LoopCount = 3);
			FLoopNodeInfo LoopNode = FLoopNodeInfo(
				FMath::FRandRange(Data.MinDelayTime,Data.MaxDelayTime),
				Data.Loop_OutPinCount,
				Data.LoopCount
			);
			USkillNode* Node =  SkillManager->NewSkillNode(LoopNode,ULoopNode::StaticClass());
			NodeData = SavedData.Emplace(GetTypeHash(Node),FNodeSaveData(Node,ESkillNodeType::LoopEndNode)); 
			return true;
		}
	case ESkillNodeType::ParamNode:
		{
			//FParamNodeInfo(EParamType _ParamType, int32 _ParamIntValue = 0, float _ParamFloatValue = 1.0f, int32 _OutPinCount = 1);
			FParamNodeInfo ParamNode = FParamNodeInfo(
				(EParamType)FMath::RandRange(1,4),
				FMath::RandRange(Data.MinParamIntValue,Data.MaxParamIntValue),
				FMath::RandRange(Data.MinParamFloatValue,Data.MaxParamFloatValue),
				Data.Params_OutPinCount
			);
			USkillNode* Node =  SkillManager->NewSkillNode(ParamNode,UParamNode::StaticClass());
			NodeData = SavedData.Emplace(GetTypeHash(Node),FNodeSaveData(Node,ESkillNodeType::ParamNode));
			return true;
		}
	case ESkillNodeType::StartNode:
		{
			//FStartNodeInfo(EStartNodeType _StartNodeType, float _DelayTime = 100.0f, int32 _OutPinCount = 1);
			int32 Count = 1;
			float Delay = 0;
			if (auto* FindCount =  Data.StartNodeOutPinCount.Find(StartType))
			{
				Count = FindCount->OutPinCount;
				Delay = FindCount->Delay;
			}
				
			FStartNodeInfo StartNode = FStartNodeInfo(
				StartType,
				Delay,
				Count
				);
			
			USkillNode* Node =  SkillManager->NewSkillNode(StartNode,UStartNode::StaticClass());
			NodeData = SavedData.Emplace(GetTypeHash(Node),FNodeSaveData(Node,ESkillNodeType::StartNode));
			return true;
		}
		default:{return false;}
	}
}

void USkillGeneratorSubsystem::AddOneNodeActorInBuildSpace(TEnumAsByte<ESkillNodeType> Type)
{
	if (Type==ESkillNodeType::StartNode)return;
	for (int32 i = 0;i< 3; i++)
	{
		for (int32 j = 0; j < 30; j++)
		{
			if (!CheckPosBlock(CenterPos+FVector(150*j,100*i,0)))
			{
				FNodeSaveData Data;
				GenerateSkills(Type,EStartNodeType::None,Data);
				SpawnNode(CenterPos+FVector(150*j,100*i,0),Data);
				return;
			}
		}
	}
	
	//GetWorld()->SpawnActorDeferred<ASkillNodeActor>(NodeActorCLass);
}

bool USkillGeneratorSubsystem::CheckPosBlock(const FVector& Pos)
{
	FHitResult Hit;
	return UKismetSystemLibrary::BoxTraceSingleForObjects(
		GetWorld(),
		Pos,
		Pos,
		FVector(150,100,500),
		{},
		CheckObjectTypes,
		false,
		{},
		EDrawDebugTrace::ForDuration,
		Hit,
		false,
		FLinearColor::Red,
		FLinearColor::Green,
		100.f
		);
}

void USkillGeneratorSubsystem::SpawnNode(const FVector& Pos,FNodeSaveData& Data)
{
	ASkillNodeActor* NodeActor = GetWorld()->SpawnActor<ASkillNodeActor>(NodeActorCLass,FTransform(Pos));
	//NodeActor->FinishSpawning(FTransform(Pos));
	NodeActor->IntialSkillNode(Data);
}
