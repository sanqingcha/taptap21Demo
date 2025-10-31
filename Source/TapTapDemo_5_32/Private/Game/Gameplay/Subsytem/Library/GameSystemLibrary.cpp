// Fill out your copyright notice in the Description page of Project Settings.


#ifdef IF_WITH_EDITOR
	#define NEED_DEBUG 0;
#endif


#include "Game/Gameplay/Subsytem/Library/GameSystemLibrary.h"


#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Game/Gameplay/Player/SuperPlayerController.h"
#include "Game/Gameplay/Player/Data/PlayerBaseData.h"
#include "Game/Gameplay/Skills/SkillNode.h"
#include "Game/Gameplay/Skills/SkillType.h"
#include "Game/Gameplay/Subsytem/Skill/SkillGeneratorSubsystem.h"
#include "Game/GameplayTags/GameTags.h"
#include "Kismet/GameplayStatics.h"
#ifdef NEED_DEBUG
#include "Kismet/KismetSystemLibrary.h"
#endif

bool UGameSystemLibrary::ApplyEffectToActor(AActor* Source, AActor* TargetActor,
	TSubclassOf<UGameplayEffect> GameplayEffectClass, float level)
{
	IAbilitySystemInterface* AbilitSysInterface = Cast<IAbilitySystemInterface>(TargetActor);
	if (!AbilitSysInterface)
	{
#ifdef NEED_DEBUG
		UKismetSystemLibrary::PrintString(TargetActor->GetWorld(),"UGameSystemLibrary::ApplyEffectToActor >> TargetActor没有实现Gas接口 ");
#endif
		return false;
	}
	UAbilitySystemComponent* ASC = AbilitSysInterface->GetAbilitySystemComponent();
	if (!ASC)
	{
#ifdef NEED_DEBUG
		UKismetSystemLibrary::PrintString(TargetActor->GetWorld(),"UGameSystemLibrary::ApplyEffectToActor >> TargetActor没有Gas组件 ");
		return false;
#endif
	}
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(Source);
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, level, ContextHandle);
	ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), ASC);
	return true;
}

void UGameSystemLibrary::Game_GetAllActorsOfClass(UObject* WorldContext, TSubclassOf<AActor> ActorClass,
	TArray<AActor*>& OutActors)
{
	UGameplayStatics::GetAllActorsOfClass(WorldContext, ActorClass, OutActors);
}

AActor* UGameSystemLibrary::SpawnActorByClass(UObject* Context, UClass* InClass, const FVector Location,
	const FRotator Rotation)
{
	return Context->GetWorld()->SpawnActor(InClass,&Location);
}

USkillGeneratorSubsystem* UGameSystemLibrary::GetSkillGenerateNodeSystem(UObject* Context)
{
	return Context->GetWorld()->GetGameInstance()->GetSubsystem<USkillGeneratorSubsystem>();
}



UPlayerBaseData* UGameSystemLibrary::GetPlayerData()
{
	return ASuperPlayerController::PlayerData_Static; 
}

const FString UGameSystemLibrary::MakeDynamicString(FString SourceString, const TArray<int32> Values)
{
	FString Result = SourceString;
    
	for (int i = 0; i < Values.Num(); i++)
	{
		FString SearchPattern = FString::Printf(TEXT("{%d}"), i); // 使用 {0}, {1}, {2}
		FString Replacement = FString::Printf(TEXT("%d"), Values[i]);
		Result.ReplaceInline(*SearchPattern, *Replacement);
	}
	return Result;
}

void UGameSystemLibrary::SetCanPlayerMove(UPlayerBaseData* Data,bool CanMove)
{
	if (CanMove)
	{
		Data->ASC->RemoveLooseGameplayTag(Game::Input::Game_Input_MoveBlock);
		
	}
	else
	{
		Data->ASC->AddLooseGameplayTag(Game::Input::Game_Input_MoveBlock);
	}
}

const FSkillNodeInfo& UGameSystemLibrary::GetNodeData(const FNodeSaveData& Data)
{
	return Data.Node->GetNodeInfo();
}

TEnumAsByte<ESkillNodeType> UGameSystemLibrary::GetSkillNodeType(const FSkillNodeInfo& Data)
{
	return Data.NodeType;
}

TEnumAsByte<ESwitchTargetType> UGameSystemLibrary::GetSwitchTargetType(const FSkillNodeInfo& Data)
{
	return Data.SwitchTargetType;
}

TEnumAsByte<EStartNodeType> UGameSystemLibrary::GetStartNodeType(const FSkillNodeInfo& Data)
{
	return Data.StartNodeType;
}

TEnumAsByte<EBranchType> UGameSystemLibrary::GetBranchType(const FSkillNodeInfo& Data)
{
	return Data.BranchType;
}

TEnumAsByte<EGeneratedType> UGameSystemLibrary::GetGeneratedType(const FSkillNodeInfo& Data)
{
	return Data.GeneratedType;
}

TEnumAsByte<EParamType> UGameSystemLibrary::GetParamType(const FSkillNodeInfo& Data)
{
	return Data.ParamType;
}

TEnumAsByte<EBuffType> UGameSystemLibrary::GetBuffType(const FSkillNodeInfo& Data)
{
	return Data.BuffType;
}



