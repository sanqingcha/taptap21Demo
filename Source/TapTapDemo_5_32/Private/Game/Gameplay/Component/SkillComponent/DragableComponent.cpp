// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Component/SkillComponent/DragableComponent.h"

UDragableComponent::UDragableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UDragableComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UDragableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

