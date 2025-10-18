// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset_StartUpDataBase.h"
#include "ActionRPG/AbilitySystem/WarriorAbilitySystemComponent.h"
#include "ActionRPG/AbilitySystem/WarriorGameplayAbility.h"


void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* WarriorASCToGive, int32 ApplyLevel)
{
	check(WarriorASCToGive);

	//부여 시 자동으로 활성화 되는 어빌리티들
	GrantAbilities(ActivateOnGivenAbilities, WarriorASCToGive, ApplyLevel);

	//특정 이벤트에 반응하는 어빌리티 목록
	GrantAbilities(ReactiveAbilities, WarriorASCToGive, ApplyLevel);

	//게임 시작 시 GamePlay Effect를 통해 캐릭터의AttributeSet초기화
	if (!StartUpGameplayEffects.IsEmpty())
	{
		//Start UP Gameplayeffect순회
		for (const TSubclassOf < UGameplayEffect >& EffectClass : StartUpGameplayEffects)
		{
			if (!EffectClass) continue;

			//UGameplayEffect 부여
			UGameplayEffect* EffectCDO = EffectClass->GetDefaultObject<UGameplayEffect>();
			WarriorASCToGive->ApplyGameplayEffectToSelf(
				EffectCDO,
				ApplyLevel,
				WarriorASCToGive->MakeEffectContext()
			);
		}
	}
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UWarriorGameplayAbility>>& InAbilitiesToGive, UWarriorAbilitySystemComponent* WarriorASCToGive, int32 ApplyLevel)
{
	if (InAbilitiesToGive.IsEmpty())
	{
		return;
	}

	//GameplayAbility 배열 순회
	//실제 어빌리티를 부여
	for (const TSubclassOf<UWarriorGameplayAbility>& Ability : InAbilitiesToGive)
	{
		if (!Ability) continue;

		//어빌리티 설정
		FGameplayAbilitySpec AbilitySpec(Ability);
		//어빌리티의 소유자
		AbilitySpec.SourceObject = WarriorASCToGive->GetAvatarActor();
		//어빌리티의 레벨 설정
		AbilitySpec.Level = ApplyLevel;

		//어빌리티 부여
		WarriorASCToGive->GiveAbility(AbilitySpec);
	}
}
