// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineVisualizeActor.generated.h"

class USplineMeshComponent;
class UNodeSplineComponent;

UCLASS()
class TAPTAPDEMO_5_32_API ASplineVisualizeActor : public AActor
{
	GENERATED_BODY()
public:
	ASplineVisualizeActor();
	virtual void BeginPlay() override;
	UNodeSplineComponent* GetOrCreateSplineComponent();
	
protected:
	void RemoveSplineComponent(UNodeSplineComponent* TargetNode);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UStaticMesh> LineMesh;
	TQueue<UNodeSplineComponent*> SplinePool;
	TSet<UNodeSplineComponent*> CurrSpline;
	
};

