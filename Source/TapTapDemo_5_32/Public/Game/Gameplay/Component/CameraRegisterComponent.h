// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CameraRegisterComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TAPTAPDEMO_5_32_API UCameraRegisterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCameraRegisterComponent();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag CameraTag;
};
