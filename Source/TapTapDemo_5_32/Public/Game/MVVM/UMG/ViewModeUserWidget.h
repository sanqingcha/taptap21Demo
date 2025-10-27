// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "ViewModeUserWidget.generated.h"

struct FOnAttributeChangeData;
class UTextBlock;
class UPlayerBaseData;
class UUMGViewModelBase;
/**
 * 
 */
UCLASS()
class TAPTAPDEMO_5_32_API UViewModeUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void Initial(UPlayerBaseData* inBaseData);
	UFUNCTION(BlueprintImplementableEvent)
	void InitialBP();
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPlayerBaseData> BaseData;
};

UCLASS()
class TAPTAPDEMO_5_32_API UPlayerWidget : public UViewModeUserWidget
{
	GENERATED_BODY()
public:
	virtual void Initial(UPlayerBaseData* inBaseData) override;
	UFUNCTION(BlueprintCallable)
	void IntialImagemat(UMaterialInterface* BloodMatTemplate,UMaterialInterface* SkillMatTemplate);
	UFUNCTION()
	void OnBloodUpdate (float Percent);
	void OnSkill1Update();
	void OnSkill2Update();
	void OnSkill3Update();
	void OnSkill4Update();
	
	UPROPERTY()
	UMaterialInstanceDynamic* BloodDyMat;
	UPROPERTY()
	UMaterialInstanceDynamic* Skill1DyMat;
	UPROPERTY()
	UMaterialInstanceDynamic* Skill2DyMat;
	UPROPERTY()
	UMaterialInstanceDynamic* Skill3DyMat;
	UPROPERTY()
	UMaterialInstanceDynamic* Skill4DyMat;
	
	
	UPROPERTY(meta = (BindWidget))
	UImage* Blood_Image;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* EnemyCount_Text;
	UPROPERTY(meta = (BindWidget))
	UImage* Skill1_Image;
	UPROPERTY(meta = (BindWidget))
	UImage* Skill2_Image;
	UPROPERTY(meta = (BindWidget))
	UImage* Skill3_Image;
	UPROPERTY(meta = (BindWidget))
	UImage* Skill4_Image;
	
};

