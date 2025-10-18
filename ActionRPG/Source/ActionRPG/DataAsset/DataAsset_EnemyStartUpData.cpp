// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset_EnemyStartUpData.h"
#include "ActionRPG/AbilitySystem/WarriorAbilitySystemComponent.h"
#include "ActionRPG/AbilitySystem/WarriorEnemyGameplayAbility.h"


void UDataAsset_EnemyStartUpData::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	if (!EnemyCombatAbilities.IsEmpty())
	{
		for (const TSubclassOf < UWarriorEnemyGameplayAbility >& AbilityClass : EnemyCombatAbilities)
		{
			if (!AbilityClass) continue;

			FGameplayAbilitySpec AbilitySpec(AbilityClass);
			AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
			AbilitySpec.Level = ApplyLevel;

			InASCToGive->GiveAbility(AbilitySpec);
		}
	}
}