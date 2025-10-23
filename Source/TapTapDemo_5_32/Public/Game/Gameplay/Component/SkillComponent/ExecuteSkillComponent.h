// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExecuteSkillComponent.generated.h"


class USkillNode;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TAPTAPDEMO_5_32_API UExecuteSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UExecuteSkillComponent();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	FTimerHandle ExecuteTimer;

	UPROPERTY()
	USkillNode* NextSkillNode;
};
