// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Component/DialoageComponent.h"

#include "Game/Gameplay/Interface/DialoageWidgetInterface.h"
#include "Game/Gameplay/Subsytem/Dialoage/DialoageSystem.h"
#include "Game/Gameplay/Subsytem/Dialoage/Data/DialoageAsset.h"


UDialoageComponent::UDialoageComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDialoageComponent::BeginPlay()
{
	Super::BeginPlay();
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	check(DialoageAsset)
	
	UDialoageSystem* DialoageSys = GetWorld()->GetSubsystem<UDialoageSystem>();
	if (!ensure(DialoageSys)){return;}
	DialoageSys->RegisterDialoageTable(DialoageAsset);
	
	DialoageSys->OnDialoageTrriggerDelegate.AddUObject(this,&UDialoageComponent::OnTrriggerDialoage);
	DialoageSys->OnDialoageBreakDelegate.AddDynamic(this,&UDialoageComponent::OnBreakDialoage);
	//DialoageAsset = DialoageSys->GetDialoageAsset();
	UUserWidget* DialoageWidget = GetWidget();
	check(DialoageWidget);
	WidgetPtr = Cast<IDialoageWidgetInterface>(DialoageWidget);
	//WidgetInterface.SetInterface(Cast<IDialoageWidgetInterface>(DialoageWidget));

	
	IDialoageWidgetInterface* Temp = WidgetPtr.Get();
	
	check(Temp);
}

void UDialoageComponent::OnTrriggerDialoage(const FGameplayTag& DialoageTag, FDialoageData* DialoageData)
{
	WidgetPtr->NewDialoage(&DialoageData->Dialoage);
}

void UDialoageComponent::OnBreakDialoage(const FGameplayTag& BreakSource, bool Force)
{
	WidgetPtr->NewDialoage({});
}
