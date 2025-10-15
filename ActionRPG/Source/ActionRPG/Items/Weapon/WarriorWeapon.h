// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WarriorWeaponBase.h"
#include "ActionRPG/WarriorTypes/WarriorStructTypes.h"
#include "WarriorWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API AWarriorWeapon : public AWarriorWeaponBase
{
	GENERATED_BODY()
	
public:
	//AnimLayer정보를 담은 구조체
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FWarriorHeroWeaponData HeroWeaponData;
};
