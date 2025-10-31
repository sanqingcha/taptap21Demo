// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Gameplay/Component/SkillComponent/ExecuteSkillComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "Game/Gameplay/Actor/SkillActor/CallBackProjectileActor.h"
#include "Game/Gameplay/Component/SkillComponent/Fragment/SkillFragmentScaleModifier.h"
#include "Game/Gameplay/Component/SkillComponent/Fragment/SkillFragmentSpeedModifier.h"
#include "Game/Gameplay/Interface/CharacterBaseInterface.h"
#include "Game/Gameplay/Player/SuperPlayerController.h"
#include "Game/Gameplay/Player/Data/PlayerBaseData.h"
#include "Game/Gameplay/Skills/SkillNode.h"
#include "Game/GameplayTags/GameTags.h"
#include "Game/Gas/Attribute/AttributeSetBase.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraSystem.h"

UExecuteSkillComponent::UExecuteSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	this->ComponentTags.Add(USkillNode::Tag_SkillComponent);
}

void UExecuteSkillComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UExecuteSkillComponent::TriggerTargetSkillList(const AActor* TargetActor)
{
	CurrentSkillNode->Delivery(TargetActor);
}

bool UExecuteSkillComponent::IfBloodLow()
{
	return false;
}

bool UExecuteSkillComponent::IfInAir() const
{
	return false;
}

void UExecuteSkillComponent::EffectBuff(FNodeBuffValueFinal BuffValueFinal)
{
}

void UExecuteSkillComponent::GenerateItem(FNodeGenerateValueFinal GenerateValueFinal)
{
}

void UExecuteSkillComponent::SetCurrentSkillNode(USkillNode* Node)
{
	CurrentSkillNode = Node;
}

void UExecuteSkillComponent::SetLoopCount(int32 LoopHashID, int32 LoopCount)
{
	EachLoopCount[LoopHashID] = LoopCount;
}

int32 UExecuteSkillComponent::GetLoopCount(int32 LoopHashID)
{
	return EachLoopCount.Contains(LoopHashID) ? EachLoopCount[LoopHashID] : 0;
}

void UExecuteSkillComponent::DecrementLoopCount(int32 LoopHashID)
{
	EachLoopCount[LoopHashID] --;
}

bool UExecuteSkillComponent::HasLoopCountKey(int32 LoopHashID)
{
	return EachLoopCount.Contains(LoopHashID);
}

void UExecuteSkillComponent::DeleteLoopCountKey(int32 LoopHashID)
{
	EachLoopCount.Remove(LoopHashID);
}

void UExecuteSkillComponent::ClearEachLoopCountMap()
{
	EachLoopCount.Empty();
}


USkillComponent::USkillComponent()
{
	
}

/***-------------------------------------------------*/
bool USkillComponent::IfBloodLow()
{
	IAbilitySystemInterface* ASCinterface = Cast<IAbilitySystemInterface>(GetOwner());
	if (!ASCinterface)return false;
	UAbilitySystemComponent* ASC = ASCinterface->GetAbilitySystemComponent();
	if (ASC->HasAttributeSetForAttribute(UPlayerAttribute::GetHealthAttribute()))
	{
		const UPlayerAttribute* AS = Cast<UPlayerAttribute>( ASC->GetAttributeSet(UPlayerAttribute::StaticClass())); 
		return AS->GetHealth()<AS->GetMaxHealth()*0.4;
	}
	if (ASC->HasAttributeSetForAttribute(UEnemyAttribute::GetHealthAttribute()))
	{
		const UEnemyAttribute* AS = Cast<UEnemyAttribute>( ASC->GetAttributeSet(UEnemyAttribute::StaticClass())); 
		return AS->GetHealth()<AS->GetMaxHealth()*0.4;
	}
	return false;
}

bool USkillComponent::IfInAir() const
{
	ICharacterBaseInterface* CharacterInterface =Cast<ICharacterBaseInterface>( GetOwner());
	if (!CharacterInterface)return false;
	return CharacterInterface->GetCharacterMovementComp()->Velocity.Z >=0.02;
}

void USkillComponent::EffectBuff(FNodeBuffValueFinal BuffValueFinal)
{
	switch (BuffValueFinal.BuffType)
	{
		case EBuffType::Accelerate:
			{

				USkillFragment_SpeedModifier* Temp = NewObject<USkillFragment_SpeedModifier>(this);
				Temp->OnTrrigger_Implementation(
				ASuperPlayerController::PlayerData_Static,
				GetOwner()->GetActorLocation(),
				BuffValueFinal,
				GetOwner());
				Temp->MarkAsGarbage();
				break;
			}
		case EBuffType::Strength:
			{
				break;
			}
	case EBuffType::AttackSpeed :
			{
				break;
			}
	case EBuffType::Size :
			{
				USkillFragment_ScaleModifier* Temp = NewObject<USkillFragment_ScaleModifier>(this);
					Temp->OnTrrigger_Implementation(
					ASuperPlayerController::PlayerData_Static,
					GetOwner()->GetActorLocation(),
					BuffValueFinal,
					GetOwner());
				Temp->MarkAsGarbage();
				break;
			}
		
	}
}

void USkillComponent::GenerateItem(FNodeGenerateValueFinal GenerateValueFinal)
{


	switch (GenerateValueFinal.GeneratedType)
	{
	case EGeneratedType::StandardBullet :
		{
			SpawnProjectile(GenerateValueFinal);
			break;
		}
	case EGeneratedType::EnhancedBullet :
		{
			SpawnProjectile(GenerateValueFinal);
			break;
		}
	case EGeneratedType::Explosion:
		{
			UAbilitySystemComponent*PlayerASC = ASuperPlayerController::PlayerData_Static->ASC;
			if(!PlayerASC)return;
			const FVector Pos = GetOwner()->GetActorLocation();
			TArray<FHitResult> HitResults; 
			if (UKismetSystemLibrary::SphereTraceMulti(
				GetWorld(),
				Pos,
				Pos,
				GenerateValueFinal.Radius,
				TraceTypeQuery1,
				false,
				{},
				EDrawDebugTrace::ForDuration,
				HitResults,
				false,
				FLinearColor::Red,
				FLinearColor::Green,
				10.f
				))
			{
				const FName tag = Game::Character::Game_Character_Enemy.GetTag().GetTagName();
				for (auto&i:HitResults)
				{
					AActor* Target = i.GetActor();
					if (Target->ActorHasTag(tag))
					{
						IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>( Target );
						if (ASCInterface)
						{
							UAbilitySystemComponent*ASC = ASCInterface->GetAbilitySystemComponent();
							if (ASC)
							{
								PlayerASC->BP_ApplyGameplayEffectToTarget(GenerateData.BoomGE,ASC,0,FGameplayEffectContextHandle());

								if (GenerateData.NS)
								{
									UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),GenerateData.NS,Target->GetActorLocation());
								}
							}
							if (Target->FindComponentByClass<USkillComponent>() != nullptr)
							{
								TriggerTargetSkillList(Target);
							}
						}
					}
				}
			};
			break;			
		}
	}
}

void USkillComponent::CallBackOnHit(AActor* Target)
{
	TriggerTargetSkillList(Target);
}

void USkillComponent::SpawnProjectile(FNodeGenerateValueFinal GenerateValueFinal)
{
	FVector Dir;
	AActor* Owner = GetOwner();
	FVector Pos ;
	if (Owner->ActorHasTag(Game::Character::Game_Character_Player.GetTag().GetTagName()))
	{
		APlayerCameraManager* Camera = ASuperPlayerController::PlayerData_Static->PC->PlayerCameraManager;
		Pos = Camera->GetCameraLocation();
		Dir = Camera->GetActorForwardVector();
	}
	else
	{
		Pos = GetOwner()->GetActorLocation();
		Dir = UKismetMathLibrary::RandomUnitVector();
	}
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(Pos);
	SpawnTransform.SetRotation(UKismetMathLibrary::MakeRotFromX(Dir).Quaternion());
	ACallBackProjectileActor* Projectile =  GetWorld()->SpawnActorDeferred<ACallBackProjectileActor>(GenerateData.ProjectileClass,SpawnTransform);
	if (Projectile)
	{
		Projectile->InitialPrevConstruct(GenerateValueFinal.GeneratedType,GetOwner());
		Projectile->OnCallbackDelegate.BindUObject(this,&USkillComponent::CallBackOnHit);
		Projectile->FinishSpawning(SpawnTransform);
	}
};
