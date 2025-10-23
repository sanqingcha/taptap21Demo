// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/DefaultWidget/SingleTextWidget.h"
#include "Game/Gameplay/DefaultWidget/PlayerDialoageWidget.h"


void USingleTextWidget::ReadOverToFall_Implementation()
{
	Owner->OnReadOverOnceDelegate.RemoveDynamic(this,&USingleTextWidget::ReadOverToFall);
	Owner->DialoageOverRightNowDelegate.RemoveDynamic(this,&USingleTextWidget::ReadOverToFall);
	/**PlayFallAnimation*/
}

void USingleTextWidget::SetText(const FString& Text, bool isLast, UPlayerDialoageWidget* TextOwner)
{
	NeedReCall = isLast;
	Owner = TextOwner;
	Owner->OnReadOverOnceDelegate.AddDynamic(this,&USingleTextWidget::ReadOverToFall);
	Owner->DialoageOverRightNowDelegate.AddDynamic(this,&USingleTextWidget::ReadOverToFall);
	SetTextBP(Text);
}

void USingleTextWidget::AnimEndCallback()
{
	if (NeedReCall)
	{
		Owner->OnReadOverOnceDelegate.Broadcast();
	}
}
