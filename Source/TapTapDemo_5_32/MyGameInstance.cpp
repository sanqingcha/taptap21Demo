// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"

void UMyGameInstance::Init()
{
    Super::Init();
}

void UMyGameInstance::OnStart()
{
    Super::OnStart();
    // GameScript = MakeShared<puerts::FJsEnv>();
    GameScript = MakeShared<puerts::FJsEnv>(std::make_unique<puerts::DefaultJSModuleLoader>(TEXT("JavaScript")), std::make_shared<puerts::FDefaultLogger>(), 8889);
    // GameScript->WaitDebugger();
    TArray<TPair<FString, UObject *>> Arguments;
    Arguments.Add(TPair<FString, UObject *>(TEXT("GameInstance"), this));
    GameScript->Start("MainGame", Arguments);
}

void UMyGameInstance::Shutdown()
{
    Super::Shutdown();
    GameScript.Reset();
}