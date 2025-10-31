// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/Gameplay/Component/NodeSplineComponent.h"
#include "Game/Gameplay/Interface/SkillNodeInteractInterface.h"
#include "Game/Gameplay/Subsytem/Skill/SkillGeneratorSubsystem.h"
#include "GameFramework/Actor.h"
#include "SkillNodeActor.generated.h"

struct FNodeSaveData;
enum class EBranchType;
enum class ESkillNodeType;
class ISkillNodeWidgetInterface;
class UWidgetComponent;
class USkillNode;

USTRUCT(BlueprintType)
struct FSkillNodeData
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly)
	FNodeSaveData NodeData ;
	uint8 MaxConnectCount = 1;
	uint32 NodeHash = 0;
	bool CanConnectHead  = false;
	
};


UCLASS()
class TAPTAPDEMO_5_32_API ASkillNodeActor : public AActor , public ISkillNodeInteractInterface
{
	GENERATED_BODY()
	
public:	
	ASkillNodeActor();
	
	UFUNCTION(BlueprintCallable)
	void IntialSkillNode(UPARAM(ref) FNodeSaveData & Node); //TODO::
	UFUNCTION(BlueprintImplementableEvent)
	void Intial();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual bool CanConnect(ESplineConnectType ConnectType) override;
	virtual void OnlyRemoveSplineData(uint32 Hash) override;
	virtual void RemoveNodeInterface() override{RemoveNode();}
	
	virtual const FVector GetHeadConnectPos() override;
	virtual const FVector GetTailConnectPos() override;
	virtual bool CanPlay(const FVector& NewPos) override;
	virtual void SetPos(const FVector& NewPos) override;
	virtual void AddSplineRef(ESplineConnectType Type,UNodeSplineComponent* SplineComp) override;
private:
	void StartMove();
	void EndMove();
	void UpdateSpline();
public:
	UFUNCTION(BlueprintCallable)
	void RemoveNode();
	
	/**在这里我使用链接的相对于这个Actor的中心位置偏移量计算的Hash值来对应链接的组件
	 * GetTypeHash(FVector)
	 * @在Actor的Transform不变的情况直接用Relative作为偏移量
	 * 生成Spline具体调用USplineSpawnSystem的SpawnSpline函数传入头和尾的Actor以及链接点的偏移量
	 * 移除Spline直接调用UNodeSplineComponent* Spline里面的DeactiveSpline就可以了，里面的委托会自动取消注册并隐藏
	 */
	//TODO::在移动这个SkillNodeActor的时候调用UpdateSpline函数
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<USceneComponent> RootSceneComp;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> NodeMeshComp;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> DetialWidgetComp;
	//ISkillNodeWidgetInterface* DetialWidget;
	
	virtual void StartTouching() override;
	virtual void EndTouching() override;
	virtual void SelectNode() override;
	virtual void UnSelectNode() override;
	virtual void StartHolding() override;
	virtual void EndHolding() override;
	
	virtual void BreakHead() override;
	virtual void ConnectFaildTips() override;
	virtual USkillNode* GetSkillNode() override{return Data.NodeData.Node;};
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void MouseTouch(bool Touch);
	UFUNCTION(BlueprintImplementableEvent)
	void SetSelectState(bool Selecting);
	
	int32 GetCurrentConnectCount();
	

	struct FConnectPackage
	{
		UNodeSplineComponent* SplineComp;
		bool HaveConnect = false;
	};

	bool isNeedUpdateSpline = false;
private:
	FConnectPackage HeadConnect;
	TMap<uint32, FConnectPackage> TailSplines;
	UPROPERTY(BlueprintReadOnly,meta = (AllowPrivateAccess = true))
	FSkillNodeData Data;
};
