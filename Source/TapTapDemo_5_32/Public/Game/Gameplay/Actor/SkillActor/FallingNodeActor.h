// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FallingNodeActor.generated.h"

enum class ESkillNodeType;
class UBoxComponent;

UCLASS()
class TAPTAPDEMO_5_32_API AFallingNodeActor : public AActor
{
	GENERATED_BODY()

public:
	AFallingNodeActor();
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TEnumAsByte<ESkillNodeType> NodeType ;
	private:
};
