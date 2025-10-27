// Fill out your copyright notice in the Description page of Project Settings.


#ifdef IF_WITH_EDITOR
	#define NEED_DEBUG 0;
#endif


#include "Game/Gameplay/Subsytem/Library/GameSystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
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


