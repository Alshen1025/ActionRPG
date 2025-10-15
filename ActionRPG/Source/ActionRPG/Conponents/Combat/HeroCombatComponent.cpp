// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroCombatComponent.h"
#include "ActionRPG/Items/Weapon/WarriorWeapon.h"

AWarriorWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(FGameplayTag WeaponTag) const
{
	return Cast<AWarriorWeapon>(GetCharacterCarriedWeaponByTag(WeaponTag));
}
