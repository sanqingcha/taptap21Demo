// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DialoageComponent.generated.h"


struct FDialoageData;
struct FGameplayTag;
class IDialoageWidgetInterface;
class UDialoageAsset;
	
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TAPTAPDEMO_5_32_API UDialoageComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UDialoageComponent();
	virtual void BeginPlay() override;
	void OnTrriggerDialoage(const FGameplayTag& DialoageTag, FDialoageData* DialoageData );
	UFUNCTION()
	void OnBreakDialoage( const FGameplayTag&  BreakSource , bool Force);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Dialoage")
	TObjectPtr<UDialoageAsset> DialoageAsset;
	//UPROPERTY(Transient)
	TObjectPtr<IDialoageWidgetInterface> WidgetPtr;
};
