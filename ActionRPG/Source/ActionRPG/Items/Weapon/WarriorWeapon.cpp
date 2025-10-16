// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorWeapon.h"

void AWarriorWeapon::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles)
{
	GrantedAbilitySpecHandles = InSpecHandles;
}

TArray<FGameplayAbilitySpecHandle> AWarriorWeapon::GetGrantedAbilitySpecHandles() const
{
	return TArray<FGameplayAbilitySpecHandle>();
}
