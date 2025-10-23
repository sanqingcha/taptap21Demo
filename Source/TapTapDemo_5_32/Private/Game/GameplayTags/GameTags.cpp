// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayTags/GameTags.h"

namespace  Game
{
	namespace Character
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Game_Character_Player,
			"Game.Character.Player",
			"Tag for Player");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Game_Character_Enemy,
			"Game.Character.Enemy",
			"Tag for Player");
	}
	namespace  Input
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Game_Input_MoveBlock,
			"Game.Input.MoveBlock",
			"Tag for PlayerInput");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Game_Input_RotateBlock,
			"Game.Input.RotatorBlock",
			"Tag for PlayerInput");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Game_Input_JumpBlock,
			"Game.Input.JumpBlock",
			"Tag for PlayerInput");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Game_Input_AttackBlock,
			"Game.Input.AttackBlock",
			"Tag for PlayerInput");
	}
}
