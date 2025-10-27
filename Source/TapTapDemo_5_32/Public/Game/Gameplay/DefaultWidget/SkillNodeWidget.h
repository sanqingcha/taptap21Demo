// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game/Gameplay/Interface/SkillNodeInteractInterface.h"
#include "Game/Gameplay/Interface/SkillNodeWidgetInterface.h"
#include "SkillNodeWidget.generated.h"

/**
 * 
 */
UCLASS()
class TAPTAPDEMO_5_32_API USkillNodeWidget : public UUserWidget,public  ISkillNodeWidgetInterface
{
	GENERATED_BODY()
};
