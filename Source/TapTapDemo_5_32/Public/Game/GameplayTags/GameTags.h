// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "NativeGameplayTags.h"

namespace  Game
{
	namespace Character
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Game_Character_Player)
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Game_Character_Enemy)
	}
	namespace  Input
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Game_Input_MoveBlock)
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Game_Input_RotateBlock)
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Game_Input_JumpBlock)
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Game_Input_AttackBlock)
	}

	namespace Actor
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Game_Actor_SkillNode)
	}

	namespace Camera
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Game_Camera_Block)
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Game_Camera_Player)
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Game_Camera_BuildSpace)
	}
}
