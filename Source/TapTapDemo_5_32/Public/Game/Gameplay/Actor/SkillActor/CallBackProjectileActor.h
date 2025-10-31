// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CallBackProjectileActor.generated.h"


class USphereComponent;
enum class EGeneratedType;

DECLARE_DELEGATE_OneParam(FOnCallback,AActor*);

UCLASS()
class TAPTAPDEMO_5_32_API ACallBackProjectileActor : public AActor
{
	GENERATED_BODY()

public:
	ACallBackProjectileActor();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereComp;
	
	void InitialPrevConstruct(EGeneratedType inType,AActor* inOwner){Type = inType;Owner = inOwner;};
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComponent ,AActor* OtherActor,  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	FOnCallback OnCallbackDelegate;
	EGeneratedType Type;
	AActor* Owner ;
};
