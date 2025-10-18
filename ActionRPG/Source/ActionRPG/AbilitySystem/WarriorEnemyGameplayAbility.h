// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WarriorGameplayAbility.h"
#include "WarriorEnemyGameplayAbility.generated.h"

class AWarriorEnemyCharacter;
class UEnemyCombatComponent;

UCLASS()
class ACTIONRPG_API UWarriorEnemyGameplayAbility : public UWarriorGameplayAbility
{
	GENERATED_BODY()
	
public:
	//어빌리티 소유 캐릭터 가져오기
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	AWarriorEnemyCharacter* GetEnemyCharacterFromActorInfo();


	//CombatComponent가져오기
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();

private:
	//Weak
	TWeakObjectPtr<AWarriorEnemyCharacter> CachedWarriorEnemyCharacter;
};
