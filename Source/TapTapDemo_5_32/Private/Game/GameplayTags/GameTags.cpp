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
			"BlockInput")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Game_Input_RotateBlock,
			"Game.Input.RotateBlock",
			"BlockInput")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Game_Input_JumpBlock,
			"Game.Input.JumpBlock",
			"BlockInput")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Game_Input_AttackBlock,
			"Game.Input.AttackBlock",
			"BlockInput")
		
		
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Game_Input_StartNode,
			"Game.Input.StartNode",
			"InputTag")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Game_Input_GenerateNode,
			"Game.Input.GenerateNode",
			"InputTag")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Game_Input_BuffNode,
			"Game.Input.BuffNode",
			"InputTag")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Game_Input_BranchNode,
			"Game.Input.BranchNode",
			"InputTag")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Game_Input_ParamNode,
			"Game.Input.ParamNode",
			"InputTag")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Game_Input_LoopStartNode,
			"Game.Input.LoopStartNode",
			"InputTag")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Game_Input_LoopEndNode,
			"GameInput_LoopEndNode",
			"InputTag") 
	}

	namespace Actor
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Game_Actor_SkillNode,
			"Game.Actor.SkillNode",
			"Tag for Mark SkillNodeActor")
	}

	namespace Camera
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Game_Camera_Block,
			"Game.Camera.Block",
			"Tag for Block CameraSwitch")
		
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Game_Camera_Player,
			"Game.Camera.Player",
			"Tag for Mark Camera")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Game_Camera_BuildSpace,
			"Game.Camera.BuildSpace",
			"Tag for Mark Camera")
	}
}
