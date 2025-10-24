// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PoolManager.generated.h"

/**
 * 
 */
#define CLASS_TO_TOSTRING(ClassName) #ClassName 

#define REGISTER_PTR_POOL(WorldContext,ClassName)	\
do												\
{													\
UWorld* _world = WorldContext->GetWorld();			\
if(!_world){check(0);break;}							\
UPoolManager* _PoolManger = _world->GetSubsystem<UPoolManager>();\
if(_PoolManger){check(0);break;}						\
_PoolManger->RegisterPtrPool<ClassName*>(CLASS_TO_TOSTRING(ClassName));\
}while(false);										\


#define GET_OR_CREATE_PTR(Ptr,Owner,ClassName) \
GetOrCreatePtr<ClassName>(Ptr,Owner,CLASS_TO_TOSTRING(ClassName))

#define DEACTIVATE_PTR(Ptr,ClassName) \
DeActivatePtr<ClassName>(Ptr,CLASS_TO_TOSTRING(ClassName)/**=CLASS_TO_TOSTRING(T)*/)

/***
 *@sanqing
 *TODO::池管理器暂不完善,可能有BUG
 *而且管理的对象要是UObject，原本用的void*，一直被GC回收,改成UObject类
 */
//UCLASS()
struct FPoolPack
{
	TSet<void*> ActivePtrs;
	TArray<UObject*> DeActivePtrs;
};

UCLASS()
class TAPTAPDEMO_5_32_API UPoolManager : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	template<typename T>
	void RegisterPtrPool(FName ClassName /**=CLASS_TO_TOSTRING(T)*/)
	{
		
		static_assert(sizeof(T*) == sizeof(UObject*), "T must be pointer-sized");
		auto* Pool = Pools.Find(ClassName);
		if (Pool == nullptr)
		{
			//UPoolPack* Poolpack = NewObject<UPoolPack>(this);
			Pools.Emplace(ClassName,FPoolPack());
		}
	};
	template<typename T,typename Outer>
	void GetOrCreatePtr(T*& Out, Outer* Owner,FName ClassName /**=CLASS_TO_TOSTRING(T)*/)
	{
	
		static_assert(sizeof(T*) == sizeof(UObject*), "T must be pointer-sized");
		auto* Pool = Pools.Find(ClassName);
		if (Pool == nullptr)
		{
			RegisterPtrPool<T>(ClassName);
			GetOrCreatePtr<T>(Out,Owner,ClassName);
			return;
		}
		if (Pool->DeActivePtrs.IsEmpty())
		{
			T* Result = NewObject<T>(Owner);
			
			Pool->ActivePtrs.Emplace(Result);
			Out = Result;
		}
		else
		{
			T* Result =	(T*)(Pool->DeActivePtrs.Pop());
			Pool->ActivePtrs.Emplace(Result);
			Out = Result;
		}
		Out->SetPoolState(true);
	}
	template<typename T>
	void DeActivatePtr(T* DeactivePtr,const FName ClassName/**=CLASS_TO_TOSTRING(T)*/)
	{
		
		static_assert(sizeof(T*) == sizeof(UObject*), "T must be pointer-sized");
		auto* Pool = Pools.Find(ClassName);
		if (Pool == nullptr)
		{
			UE_LOG(LogTemp,Error,TEXT("DeActivatePtr::NoPool"));
			check(!(T::StaticClass()));
			return;
		}
		T* PtrSave = (T*) (*(Pool->ActivePtrs.Find(DeactivePtr)));
		if (PtrSave == nullptr)
		{
			UE_LOG(LogTemp,Error,TEXT("DeActivatePtr::NoPtrInPool"));
			return;
		}
		PtrSave->SetPoolState(false);
		Pool->ActivePtrs.Remove(PtrSave);
		Pool->DeActivePtrs.Add(DeactivePtr);
	};

private:
	TMap<FName,FPoolPack> Pools;
};
