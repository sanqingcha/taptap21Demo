// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerPawnInterface.generated.h"

DECLARE_DELEGATE_OneParam(FOnInputChanged,bool Activate);

class USpringArmComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI,BlueprintType)
class UPlayerPawnInterface : public UInterface
{
	GENERATED_BODY()
};

class TAPTAPDEMO_5_32_API IPlayerPawnInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	USpringArmComponent* GetSpringArmComponent();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CallJump();

	virtual FOnInputChanged& GetInputChangedDelegate() = 0;
};
