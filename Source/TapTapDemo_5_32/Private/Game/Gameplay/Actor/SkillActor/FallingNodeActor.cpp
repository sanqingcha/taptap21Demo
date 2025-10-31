// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Actor/SkillActor/FallingNodeActor.h"
#include "Game/Gameplay/Skills/SkillType.h"
#include "Game/Gameplay/Subsytem/Skill/SkillGeneratorSubsystem.h"
#include "Game/GameplayTags/GameTags.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AFallingNodeActor::AFallingNodeActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("RootSceneRoot"));
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComp->SetupAttachment(GetRootComponent());

	StaticMeshComp->OnComponentBeginOverlap.AddDynamic(this,&AFallingNodeActor::OnBeginOverlap);
}
void AFallingNodeActor::BeginPlay()
{
	Super::BeginPlay();

	NodeType = (ESkillNodeType)FMath::RandRange(2,6);
	
#ifdef  IF_WITH_EDITOR
	UE_LOG(LogTemp,Display,TEXT("AFallingNodeActor::BeginPlay::SpawnType  == ESkillNodeType :: %s"),*UEnum::GetValueAsString(NodeType));
#endif
	
	FHitResult OutHit;
	if (UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation()+FVector(0,0,-10000),
		50,
		TraceTypeQuery1,
		false,
		{},
		EDrawDebugTrace::ForDuration,
		OutHit,
		false,
		FLinearColor::Red,
		FLinearColor::Green,
		10.f
		))
	{
		SetActorLocation(OutHit.Location);
	};
	
}

void AFallingNodeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFallingNodeActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(Game::Character::Game_Character_Player.GetTag().GetTagName()))
	{
		GetGameInstance()->GetSubsystem<USkillGeneratorSubsystem>()->AddOneNodeActorInBuildSpace(NodeType);
		Destroy();
	};
}


