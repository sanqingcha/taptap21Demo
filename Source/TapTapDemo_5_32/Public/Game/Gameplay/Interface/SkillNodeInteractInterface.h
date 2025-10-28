// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SkillNodeInteractInterface.generated.h"

class UNodeSplineComponent;
class USkillNode;
enum class ESplineConnectType : uint8;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USkillNodeInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TAPTAPDEMO_5_32_API ISkillNodeInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void StartTouching() = 0; //
	virtual void EndTouching() = 0;  //

	virtual void SelectNode() = 0;  //
	virtual void UnSelectNode() = 0; //
	
	virtual void StartHolding() = 0;
	virtual void EndHolding() = 0;

	virtual void ConnectFaildTips() = 0 ; //
	
	virtual bool CanConnect(ESplineConnectType ConnectType) = 0; //
	virtual void OnlyRemoveSplineData(uint32 Hash) = 0; //
	virtual void BreakHead() = 0; //
	virtual void RemoveNodeInterface() = 0; //直接移除整个Node
	

	virtual USkillNode* GetSkillNode() = 0;//
	
	
	virtual const FVector GetHeadConnectPos() = 0; //
	virtual const FVector GetTailConnectPos() = 0; //
	virtual bool CanPlay(const FVector& NewPos) = 0; //
	virtual void SetPos(const FVector& NewPos) = 0; //

	virtual void AddSplineRef(ESplineConnectType Type,UNodeSplineComponent* SplineComp) = 0;//
	
};
