// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MVVM/UMG/ViewModeUserWidget.h"

#include "AbilitySystemComponent.h"
#include "Game/Gameplay/Player/Data/PlayerBaseData.h"
#include "Game/Gas/Attribute/AttributeSetBase.h"
#include "Kismet/KismetMaterialLibrary.h"


void UViewModeUserWidget::Initial(UPlayerBaseData* inBaseData)
{
	BaseData = inBaseData;
	InitialBP();
}

void UPlayerWidget::Initial(UPlayerBaseData* inBaseData)
{
	Super::Initial(inBaseData);
}

void UPlayerWidget::IntialImagemat(UMaterialInterface* BloodMatTemplate, UMaterialInterface* SkillMatTemplate)
{
	UWorld* CurrWorld = GetWorld();
	BloodDyMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(CurrWorld,BloodMatTemplate);
	Blood_Image->SetBrushFromMaterial(BloodDyMat);

	Skill1DyMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(CurrWorld,SkillMatTemplate);
	Skill1_Image->SetBrushFromMaterial(Skill1DyMat);
	Skill2DyMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(CurrWorld,SkillMatTemplate);
	Skill2_Image->SetBrushFromMaterial(Skill2DyMat);
	Skill3DyMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(CurrWorld,SkillMatTemplate);
	Skill3_Image->SetBrushFromMaterial(Skill3DyMat);
	Skill4DyMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(CurrWorld,SkillMatTemplate);
	Skill4_Image->SetBrushFromMaterial(Skill4DyMat);
	check(BaseData)
	const UPlayerAttribute* PlayerAttribute =  Cast<UPlayerAttribute>(BaseData->ASC->GetAttributeSet(UPlayerAttribute::StaticClass()));
	UPlayerAttribute* MutPlayerAttribute = const_cast<UPlayerAttribute*>(PlayerAttribute);
	OnBloodUpdate(MutPlayerAttribute->GetHPPercent());
	MutPlayerAttribute->OnHPPercentChangeDelegate.AddUObject(this,&UPlayerWidget::OnBloodUpdate);
}

void UPlayerWidget::OnBloodUpdate(float Percent)
{
	BloodDyMat->SetScalarParameterValue("Percent",Percent);
}

void UPlayerWidget::OnSkill1Update()
{
	
}

void UPlayerWidget::OnSkill2Update()
{
	
}

void UPlayerWidget::OnSkill3Update()
{
	
}

void UPlayerWidget::OnSkill4Update()
{
	
}
