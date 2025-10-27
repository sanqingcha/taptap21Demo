// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DialoageSystem.generated.h"

struct FDialoageData;
class UDialoageAsset;
struct FGameplayTag;

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTrriggerDialoage, const FGameplayTag& , FDialoageData* );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBreakDialoage, const FGameplayTag& ,BreakSource,bool , Force);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAssetLoad,UDialoageAsset* , Asset);

UCLASS()
class TAPTAPDEMO_5_32_API UDialoageSystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	FOnTrriggerDialoage OnDialoageTrriggerDelegate;
	UPROPERTY(BlueprintAssignable,BlueprintReadWrite,Category = "Dialoage")
	FOnBreakDialoage OnDialoageBreakDelegate;
	
	/**用于吧资产表传给所有触发器*/
	FOnAssetLoad OnAssetLoadDelegate;
	
	/**在玩家对话组件中初始化Asset*/
	UFUNCTION(BlueprintCallable, Category = "Dialoage")
	void RegisterDialoageTable(UDialoageAsset*inDialoageAsset);
	void LoadAsset(UDialoageAsset* Asset);
	
	UFUNCTION(BlueprintCallable, Category = "Dialoage")
	void TrriggerDialoage(const FGameplayTag& TrriggerTag);
	/**打断对话*/
	void BreakDialoage(const FGameplayTag& DialoageKey/**打断源*/);
	/**无论是什么对话，立即终止*/
	UFUNCTION(BlueprintCallable, Category = "Dialoage")
	void BreakAllDialoage();

	UDialoageAsset* GetDialoageAsset(){return DialoageAsset;}

	
private:
	UPROPERTY()
	TObjectPtr<UDialoageAsset> DialoageAsset;
};
