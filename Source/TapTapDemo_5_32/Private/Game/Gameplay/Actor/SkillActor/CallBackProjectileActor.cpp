// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Actor/SkillActor/CallBackProjectileActor.h"

#include "Components/SphereComponent.h"
#include "Game/GameplayTags/GameTags.h"


ACallBackProjectileActor::ACallBackProjectileActor()
{
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComponent"));

	
}

void ACallBackProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&ACallBackProjectileActor::OnHit);
	
}
void ACallBackProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACallBackProjectileActor::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Owner==OtherActor)return;
	if (OtherActor->ActorHasTag(Game::Character::Game_Character_Enemy.GetTag().GetTagName()))
	{
		if (OnCallbackDelegate.IsBound())
		{
			OnCallbackDelegate.Execute(OtherActor);
			Destroy();
		}
	}
}

