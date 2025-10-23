// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "NodeSplineComponent.generated.h"

class INodeSplineInterface;

UENUM(BlueprintType)
enum class ESplineConnectType : uint8
{
	Head = 0,
	Tail = 1,
};

USTRUCT(BlueprintType)
struct FNodeConnectData
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	AActor* ConnectActor;
	UPROPERTY(BlueprintReadWrite)
	FVector ConnectOffset;
};

USTRUCT(BlueprintType)
struct FSplineConnectData
{
	GENERATED_BODY()
	class UNodeSplineComponent * NodeSplineComp;
	ESplineConnectType ConnectType;
};



DECLARE_DELEGATE_OneParam(FOnDeactive, class UNodeSplineComponent*);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TAPTAPDEMO_5_32_API UNodeSplineComponent : public USplineComponent
{
	GENERATED_BODY()
public:
	UNodeSplineComponent();
	virtual void BeginPlay() override;

	void UpdateSplineForConnect();
	void ActiveSpline(const FNodeConnectData& Head,const FNodeConnectData& Tail);
	void DeactiveSpline();
private:
	void HideSpline();
	void ShowSpline();
	/**每个线条只会有2个链接位置*/
	TStaticArray<FNodeConnectData,2> ConnectNodeData;
public:
	FOnDeactive OnDeactive;
};
