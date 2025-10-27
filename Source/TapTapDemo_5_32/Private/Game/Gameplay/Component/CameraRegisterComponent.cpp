// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Component/CameraRegisterComponent.h"

#include "Game/Gameplay/Subsytem/GISubSystem/MapSteamLoadSystem.h"

UCameraRegisterComponent::UCameraRegisterComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UCameraRegisterComponent::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetGameInstance()->GetSubsystem<UMapSteamLoadSystem>()->RegisterCamera(CameraTag,GetOwner());
}

