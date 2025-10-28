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
#include "Game/Gameplay/Subsytem/GISubSystem/SoundSettingSubsystem.h"
#include "Game/Gameplay/Subsytem/Skill/SplineSpawnSystem.h"
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

	UGameSettingSubsystem* GameSettingSubsystem = GetGameInstance()->GetSubsystem<UGameSettingSubsystem>();
	CheckTime = GameSettingSubsystem->GetCheckTime();
	GameSettingSubsystem->GetCheckTimeDelegate().BindLambda([this](float Newvalue){CheckTime = Newvalue;});
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

void ASingleCameraActor::Pressed_Action(const FInputActionValue& Value)
{
	if (CurrMouseNode)
	{
		CurrNodePos = GetActorLocation();
		FollowNodeInterface = Cast<ISkillNodeInteractInterface>(CurrMouseNode);
	}
	if (!isActive)return;
	PressedIsVaild = false;
	ReleasedIsVaild = false;
	FTimerHandle PressedCheckTimer;
	GetWorld()->GetTimerManager().SetTimer(
			PressedCheckTimer,
			this,
			&ASingleCameraActor::DelayCheck,
			CheckTime,
			false);
}

void ASingleCameraActor::NodeFollowing()
{
	UWorld* mWorld = GetWorld();
	const FVector2D ViewPos =  UWidgetLayoutLibrary::GetMousePositionOnViewport(mWorld)*UWidgetLayoutLibrary::GetViewportScale(mWorld);
	FVector WorldPos = FVector::ZeroVector;
	FVector WorldDirection = FVector::ZeroVector;
	UGameplayStatics::DeprojectScreenToWorld(PlayerData->PC,ViewPos,WorldPos,WorldDirection);
	float Z = GetActorLocation().Z;
	const FVector NewPos = FVector(WorldPos.X+(WorldDirection.X*(Z-WorldPos.Z))/WorldDirection.Z,WorldPos.Y+(WorldDirection.Y*(Z-WorldPos.Z))/WorldDirection.Z,Z);
	if (FollowNodeInterface->CanPlay(NewPos))
	{
		CurrNodePos = NewPos;
	}
	FollowNodeInterface->SetPos(NewPos);
}

void ASingleCameraActor::DelayCheck()
{
	if (!ReleasedIsVaild)
	{
		ToHoldingNodeActor();
		PressedIsVaild = true;
	};
}

void ASingleCameraActor::Release_Action(const FInputActionValue& Value)
{
	ReleasedIsVaild = true;
	if (PressedIsVaild)
	{
		ToReleaseNodeAction();
	}
	else
	{
		//TODO::Click
		
		AActor* SelectNode = CurrMouseNode;
		ISkillNodeInteractInterface* SkillNode = Cast<ISkillNodeInteractInterface>(SelectNode);

		if (!SkillNode)return;
		
		if (ShiftPressed)
		{
			SkillNode->BreakHead();
			return;
		}
		
		bool NeedConnect = false;
		if (SelectHead==nullptr)
		{
			SelectHead = SkillNode;
			SelectHead->SelectNode();
		}
		else
		{
			do
			{
				if (SelectHead==SkillNode)
				{
					SelectHead->UnSelectNode();
					SelectHead=nullptr;
					break;
				}
				if (SelectTail==nullptr)
				{
					SelectTail=SkillNode;
					SelectTail->SelectNode();
					NeedConnect = true;
					break;
				}
				if (SelectTail==SkillNode)
				{
					/**这个分支应该不会被调用*/ //TODO::
					SelectTail->UnSelectNode();
					SelectTail=nullptr;
					break;
				}
				
			}while(false);
		}
		if (NeedConnect)
		{
			USplineSpawnSystem* SplineSysComp = GetWorld()->GetSubsystem<USplineSpawnSystem>();
			bool SuccessSpawn = false;
			SplineSysComp->TryConnect(SelectHead,SelectTail,SuccessSpawn);
			SelectHead->UnSelectNode();
			SelectTail->UnSelectNode();
			if (!SuccessSpawn)
			{
				SelectHead->ConnectFaildTips();
				SelectTail->ConnectFaildTips();
			}
			SelectHead = nullptr;
			SelectTail=nullptr;
		}
	}
}

void ASingleCameraActor::ToHoldingNodeActor()
{
	//TODO::停止检测节点信息，并且node开始跟随;
	if (FollowNodeInterface)
	{
		isHolding= true;
		//FollowNodeInterface = NodeInterface;
		FollowNodeInterface->EndTouching();
		FollowNodeInterface->StartHolding();
		if (HoldTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().UnPauseTimer(HoldTimerHandle);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(
				HoldTimerHandle,
				this,
				&ASingleCameraActor::NodeFollowing,
				0.02,
				true);
		}
	}
}

void ASingleCameraActor::ToReleaseNodeAction()
{
	GetWorld()->GetTimerManager().PauseTimer(HoldTimerHandle);
	if (FollowNodeInterface)
	{
		FollowNodeInterface->SetPos(CurrNodePos);
		FollowNodeInterface->EndHolding();
	}
	isHolding = false;
	//TODO::node停止跟随actor，并且开始检测节点信息
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
			if (HitActor!=CurrMouseNode && HitActor->ActorHasTag(Game::Actor::Game_Actor_SkillNode.GetTag().GetTagName()))
			{
				if (HitActor==CurrMouseNode)return;
				if (CurrMouseNode)
				{
					if ( ISkillNodeInteractInterface* Node =  Cast<ISkillNodeInteractInterface>(CurrMouseNode))
					{
						Node ->EndTouching();
					}
				}
				CurrMouseNode = HitActor;
				if (ISkillNodeInteractInterface* Node =  Cast<ISkillNodeInteractInterface>(CurrMouseNode))
				{
					Node ->StartTouching();
				}
			}
		}
		else
		{
			if (CurrMouseNode)
			{
				if (ISkillNodeInteractInterface* Node =  Cast<ISkillNodeInteractInterface>(CurrMouseNode))
				{
					Node ->EndTouching();
					CurrMouseNode = nullptr;
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
	PlayerData->EnhancedInputComp->BindAction(IA_Hold, ETriggerEvent::Started, this, &ASingleCameraActor::Pressed_Action);
	PlayerData->EnhancedInputComp->BindAction(IA_Hold, ETriggerEvent::Completed, this, &ASingleCameraActor::Release_Action);
	PlayerData->EnhancedInputComp->BindAction(IA_Hold, ETriggerEvent::Canceled, this, &ASingleCameraActor::Release_Action);
	PlayerData->EnhancedInputComp->BindAction(IA_Shift_Modifier, ETriggerEvent::Started, this, &ASingleCameraActor::ShiftModifier,true);
	PlayerData->EnhancedInputComp->BindAction(IA_Shift_Modifier, ETriggerEvent::Canceled, this, &ASingleCameraActor::ShiftModifier,false);
	PlayerData->EnhancedInputComp->BindAction(IA_Shift_Modifier, ETriggerEvent::Completed, this, &ASingleCameraActor::ShiftModifier,false);
	
}



