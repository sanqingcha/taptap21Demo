// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/DefaultWidget/PlayerDialoageWidget.h"

#include "Components/AudioComponent.h"
#include "Game/Gameplay/DefaultWidget/SingleTextWidget.h"
#include "Game/Gameplay/Subsytem/Dialoage/Data/DialoageAsset.h"
#include "Game/Gameplay/Subsytem/GISubSystem/SoundSettingSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UPlayerDialoageWidget::NativeConstruct()
{
	Super::NativeConstruct();
	OnReadOverOnceDelegate.AddDynamic(this,&UPlayerDialoageWidget::ReadOverCallback);
	SoundSettingSubsys = GetGameInstance()->GetSubsystem<USoundSettingSubsystem>();
	check(SoundSettingSubsys);
}

void UPlayerDialoageWidget::NewDialoage(TArray<FOnceDialoageData>* Dialoage)
{
	for (auto&i: *Dialoage)
	{
		i.SoundSource = i.SoundSoft.LoadSynchronous();
	}
	DialoageOverRightNowDelegate.Broadcast();
	DialoageArray = Dialoage;
	CurrIndex = 0;
	ReadOnce();
}

void UPlayerDialoageWidget::ReadOnce()
{
	if (DialoageArray->IsValidIndex(CurrIndex)&&(!isDelaying))
	{
		isDelaying = true;
		FOnceDialoageData* Data =  &(*DialoageArray)[CurrIndex];
		
		FTimerHandle TimerHandle;
		FTimerDelegate Delegate;

		Delegate.BindLambda([Data,this]()
		{
			ReadOncePlayAnim((Data->Dialoage));
			if (Data->SoundSource!=nullptr)
			{
				if (LastSound)
				{
					LastSound->Stop();
					LastSound->MarkAsGarbage();
				}
				LastSound = UGameplayStatics::SpawnSound2D(GetWorld(),Data->SoundSource,SoundSettingSubsys->GetSoundSettings().DialogueVolume);
			}
			CurrIndex++;
			isDelaying = false;
			});
		
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			Delegate,
			Data->Delay,
			false);
	}
}

void UPlayerDialoageWidget::ReadOncePlayAnim(FString& CurrDialoage)
{
	CurrTextIndex = 0;
	CurrText = CurrDialoage;
	if (ReadTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().UnPauseTimer(ReadTimerHandle);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(
			ReadTimerHandle,
			this,
			&UPlayerDialoageWidget::ReadOneText,
			ReadSpeed,
			true);
	}
}

void UPlayerDialoageWidget::ReadOverCallback()
{
	ReadOnce();
}

void UPlayerDialoageWidget::ReadOneText()
{
	if (CurrText.IsValidIndex(CurrTextIndex))
	{
		FString OneText ;
		OneText.AppendChar(CurrText[CurrTextIndex]);
		bool isLast = (++CurrTextIndex)==CurrText.Len();
		CreateOneText(OneText,isLast);
		if (!isLast) return;
	}
	/**在if中没有return，或者没有进入if里面，没有字符需要生成了，就直接中断计时器*/
	UE_LOG(LogTemp,Error,TEXT("Read Over Text Once"));
	GetWorld()->GetTimerManager().PauseTimer(ReadTimerHandle);
}

void UPlayerDialoageWidget::CreateOneText(const FString& Text, bool isLast)
{
	check(!TextWidgetClass.IsNull());
	USingleTextWidget* SingleText = CreateWidget<USingleTextWidget>(this,TextWidgetClass.LoadSynchronous());
	SingleText->SetText(Text,isLast,this);
	CreateOneTextBP(SingleText);
}



