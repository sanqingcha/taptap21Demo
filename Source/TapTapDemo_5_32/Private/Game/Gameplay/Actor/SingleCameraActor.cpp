// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Actor/SingleCameraActor.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Camera/CameraComponent.h"
#include "Game/Gameplay/Component/CameraRegisterComponent.h"
#include "Game/Gameplay/Interface/SkillNodeInteractInterface.h"
#include "Game/Gameplay/Player/Data/PlayerBaseData.h"
#include "Game/GameplayTags/GameTags.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

ASingleCameraActor::ASingleCameraActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp")));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(GetRootComponent());
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	CameraRegisterCompoent = CreateDefaultSubobject<UCameraRegisterComponent>(TEXT("CameraRegisterComp"));

	OnActivateInputDelegate.AddDynamic(this,&ASingleCameraActor::BindMapping);
}
void ASingleCameraActor::BeginPlay()
{
	Super::BeginPlay();

	SpringLengthTarget = SpringArmComp->TargetArmLength;
	check(!CheckObjectTypes.IsEmpty());
	
}
void ASingleCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateInputControl(DeltaTime);
}

void ASingleCameraActor::Move_Action(const FInputActionValue& Value)
{
	if (!isActive)return;
	UAbilitySystemComponent* AbilitySysComp = PlayerData->ASC;
	if (AbilitySysComp&&AbilitySysComp->HasMatchingGameplayTag(Game::Input::Game_Input_MoveBlock)){return;}
	const FVector2D InputValue = Value.Get<FVector2D>()*GetWorld()->GetDeltaSeconds()*Speed;
	FVector CurrPos = SpringArmComp->GetComponentLocation();
	//UKismetSystemLibrary::PrintString(GetWorld(),FString::Printf(TEXT("ForPos = %s"),*CurrPos.ToString()),true,false,FLinearColor::Red,GetWorld()->GetDeltaSeconds());
	bool NeedUpdate = false;
	const FVector CenterPos = GetActorLocation();
	if (InputValue.X!=0 && abs(CurrPos.X+InputValue.X-CenterPos.X) < MoveRangeQuare)
	{
		NeedUpdate = true;
		CurrPos.X += InputValue.X;
	}
	if (InputValue.Y!=0 && abs(CurrPos.Y+InputValue.Y-CenterPos.Y) < MoveRangeQuare)
	{
		CurrPos.Y += InputValue.Y;
		NeedUpdate = true;
	}
	if (NeedUpdate)
	{
		//UKismetSystemLibrary::PrintString(GetWorld(),FString::Printf(TEXT("OffestPos = %s"),*InputValue.ToString()),true,false,FLinearColor::Red,GetWorld()->GetDeltaSeconds());
		//UKismetSystemLibrary::PrintString(GetWorld(),FString::Printf(TEXT("EndPos = %s"),*CurrPos.ToString()),true,false,FLinearColor::Red,GetWorld()->GetDeltaSeconds());
		SpringArmComp->SetWorldLocation(CurrPos);
	}
}

void ASingleCameraActor::Zoom_Action(const FInputActionValue& Value)
{
	if (!isActive)return;
	UAbilitySystemComponent* AbilitySysComp = PlayerData->ASC;
	if (AbilitySysComp&&AbilitySysComp->HasMatchingGameplayTag(Game::Input::Game_Input_MoveBlock)){return;}
	const float InputValue = Value.Get<float>();

	const float NewLength = SpringArmComp->TargetArmLength+ InputValue*ZoomSpeed; 
	if (NewLength < MaxSpringLength&&NewLength>MinSpringLenhth)
	{
		SpringLengthTarget = NewLength;
	}
}

void ASingleCameraActor::Hold_Action(const FInputActionValue& Value)
{
	if (!isActive)return;
	/*UAbilitySystemComponent* AbilitySysComp = PlayerData->ASC;
	if (AbilitySysComp&&AbilitySysComp->HasMatchingGameplayTag(Game::Input::Game_Input_MoveBlock)){return;}

	UWorld* mWorld = GetWorld();
	FVector2D ViewPos =  UWidgetLayoutLibrary::GetMousePositionOnViewport(mWorld)*UWidgetLayoutLibrary::GetViewportScale(mWorld);
	FVector WorldPos = FVector::ZeroVector;
	FVector WorldDirection = FVector::ZeroVector;
	UGameplayStatics::DeprojectScreenToWorld(PlayerData->PC,ViewPos,WorldPos,WorldDirection);

	FHitResult OutHit;
	
	if ( UKismetSystemLibrary::LineTraceSingle(
		mWorld,
		WorldPos,
		WorldDirection*10000,
		TraceTypeQuery1,
		false,
		{},
		EDrawDebugTrace::ForDuration,
		OutHit,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		GetWorld()->GetDeltaSeconds()
		))
	{
		//UKismetSystemLibrary::PrintString(GetWorld(),FString::Printf(TEXT("Name = %s"),*OutHit.GetActor()->GetName()),true,false,FLinearColor::Red,GetWorld()->GetDeltaSeconds());
		FString HasLog = OutHit.GetActor()->ActorHasTag(Game::Actor::Game_Actor_SkillNode.GetTag().GetTagName())?"True" : "false";
		//UKismetSystemLibrary::PrintString(GetWorld(),FString::Printf(TEXT("HasSkillTag = %s"),*HasLog),true,false,FLinearColor::Red,GetWorld()->GetDeltaSeconds());
	}*/
}

void ASingleCameraActor::Holding_Action(const FInputActionValue& Value)
{
	if (!isActive)return;
	if (HoldingNode)
	{
		
	}
}

void ASingleCameraActor::Release_Action(const FInputActionValue& Value)
{
	HoldingNode = nullptr;
}

void ASingleCameraActor::UpdateInputControl(float DeltaTime)
{
	if (isActive)
	{
		UAbilitySystemComponent* AbilitySysComp = PlayerData->ASC;
		if (AbilitySysComp&&AbilitySysComp->HasMatchingGameplayTag(Game::Input::Game_Input_MoveBlock)){return;}
		UpdateSpringArmLength(DeltaTime);
		MouseFloatingAction(DeltaTime);
	}
}

void ASingleCameraActor::MouseFloatingAction(float DeltaTime)
{
	if (!isHolding)
	{
		UWorld* mWorld = GetWorld();
		const FVector2D ViewPos =  UWidgetLayoutLibrary::GetMousePositionOnViewport(mWorld)*UWidgetLayoutLibrary::GetViewportScale(mWorld);
		FVector WorldPos = FVector::ZeroVector;
		FVector WorldDirection = FVector::ZeroVector;
		UGameplayStatics::DeprojectScreenToWorld(PlayerData->PC,ViewPos,WorldPos,WorldDirection);

		FHitResult OutHit;
		if (UKismetSystemLibrary::LineTraceSingleForObjects(
			mWorld,
			WorldPos,
			WorldPos+WorldDirection*10000,
			CheckObjectTypes,
			false,
			{},
			EDrawDebugTrace::ForDuration,
			OutHit,
			false,
			FLinearColor::Red,
			FLinearColor::Green,
			GetWorld()->GetDeltaSeconds()
			))
		{
			AActor* HitActor = OutHit.GetActor();
			if (!HitActor)return;
			if (HitActor!=HoldingNode && HitActor->ActorHasTag(Game::Actor::Game_Actor_SkillNode.GetTag().GetTagName()))
			{
				if (HitActor==HoldingNode)return;
				if (HoldingNode)
				{
					if ( ISkillNodeInteractInterface* Node =  Cast<ISkillNodeInteractInterface>(HoldingNode))
					{
						Node ->EndTouching();
					}
				}
				HoldingNode = HitActor;
				if (ISkillNodeInteractInterface* Node =  Cast<ISkillNodeInteractInterface>(HoldingNode))
				{
					Node ->StartTouching();
				}
			}
		}
		else
		{
			if (HoldingNode)
			{
				if (ISkillNodeInteractInterface* Node =  Cast<ISkillNodeInteractInterface>(HoldingNode))
				{
					Node ->EndTouching();
					HoldingNode = nullptr;
				}
			}
		}
	}
}

void ASingleCameraActor::UpdateSpringArmLength(float DeltaTime)
{
	SpringArmComp->TargetArmLength =  0.3*SpringArmComp->TargetArmLength + 0.7*SpringLengthTarget;
}

void ASingleCameraActor::ActivateCamera(UPlayerBaseData* inPlayerData)
{
	SpringArmComp->SetRelativeLocation({});
	PlayerData = inPlayerData;
	PlayerData->EnhancedInputSys->AddMappingContext(Mapping,0);
	OnActivateInputDelegate.Broadcast(true);
	PlayerData->PC->SetShowMouseCursor(true);
	isActive = true;
}

void ASingleCameraActor::DeactivateCamera(UPlayerBaseData* inPlayerData)
{
	isActive = false;
	PlayerData = inPlayerData;
	PlayerData->EnhancedInputSys->RemoveMappingContext(Mapping);
	OnActivateInputDelegate.Broadcast(false);
	PlayerData->PC->SetShowMouseCursor(false);
}

void ASingleCameraActor::BindMapping(bool isActivate)
{
	OnActivateInputDelegate.RemoveDynamic(this,&ASingleCameraActor::BindMapping);
	PlayerData->EnhancedInputComp->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ASingleCameraActor::Move_Action);
	PlayerData->EnhancedInputComp->BindAction(IA_Zoom, ETriggerEvent::Triggered, this, &ASingleCameraActor::Zoom_Action);
	PlayerData->EnhancedInputComp->BindAction(IA_Hold, ETriggerEvent::Started, this, &ASingleCameraActor::Hold_Action);
	PlayerData->EnhancedInputComp->BindAction(IA_Hold, ETriggerEvent::Triggered, this, &ASingleCameraActor::Holding_Action);
	PlayerData->EnhancedInputComp->BindAction(IA_Hold, ETriggerEvent::Completed, this, &ASingleCameraActor::Release_Action);
	PlayerData->EnhancedInputComp->BindAction(IA_Hold, ETriggerEvent::Canceled, this, &ASingleCameraActor::Release_Action);
	
}

