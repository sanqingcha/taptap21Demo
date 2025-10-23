// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/Gameplay/Component/NodeSplineComponent.h"
#include "Game/Gameplay/Interface/NodeSplineInterface.h"
#include "GameFramework/Actor.h"
#include "SkillNodeActor.generated.h"

class USkillNode;

UCLASS()
class TAPTAPDEMO_5_32_API ASkillNodeActor : public AActor , public INodeSplineInterface
{
	GENERATED_BODY()
	
public:	
	ASkillNodeActor();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<USkillNode> LogicalNode;

	UPROPERTY()
	int32 HashID;

public:
	/**由外部调用过来*/
	virtual void SetSpline(const FSplineConnectData& Data, uint32 Hash) override;
	virtual void RemoveSpline(const uint32 ConnectPosHash) override;
	void UpdateSpline();
	/**在这里我使用链接的相对于这个Actor的中心位置偏移量计算的Hash值来对应链接的组件
	 * GetTypeHash(FVector)
	 * @在Actor的Transform不变的情况直接用Relative作为偏移量
	 * 生成Spline具体调用USplineSpawnSystem的SpawnSpline函数传入头和尾的Actor以及链接点的偏移量
	 * 移除Spline直接调用UNodeSplineComponent* Spline里面的DeactiveSpline就可以了，里面的委托会自动取消注册并隐藏
	 */
	//TODO::在移动这个SkillNodeActor的时候调用UpdateSpline函数
	TMap<uint32, FSplineConnectData> Splines;
};
