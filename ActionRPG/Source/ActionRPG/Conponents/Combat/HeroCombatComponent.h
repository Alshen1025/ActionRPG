// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRPG/Conponents/Combat/PawnCombatComponent.h"
#include "HeroCombatComponent.generated.h"

class AWarriorWeapon;

UCLASS()
class ACTIONRPG_API UHeroCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorWeapon* GetHeroCarriedWeaponByTag(FGameplayTag WeaponTag) const;

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorWeapon* GetHeroCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	float GetHeroCurrentEquippWeaponDamageAtLevel(float InLevel) const;

	virtual void OnHitTargetActor(AActor* HitActor) override;

	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor) override;
};
