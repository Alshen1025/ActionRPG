// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "NativeGameplayTags.h"


namespace WarriorGameplayTags
{
	//Input 관련 Tags
	ACTIONRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	ACTIONRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);
	ACTIONRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_EquipAxe);
	ACTIONRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UnequipAxe);

	ACTIONRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LightAttack_Axe);

	ACTIONRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_HeavyAttack_Axe);
	//Player 관련 Tag
	ACTIONRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Axe);

	//Equipping
	ACTIONRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Equip_Axe);
	ACTIONRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Unequip_Axe);

	//Attack

	ACTIONRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Light_Axe);
	ACTIONRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Heavy__Axe);

	//GamplayEvent 관련
	ACTIONRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equip_Axe);
	ACTIONRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Unequip_Axe);

	//Status
	ACTIONRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_JumpToFinisher);

	//적대 몬스터
	ACTIONRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Weapon);

	//Public Tag
	ACTIONRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Public_Event_MeleeHit);
}