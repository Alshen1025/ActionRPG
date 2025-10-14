// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset_StartUpDataBase.h"
#include "ActionRPG/AbilitySystem/WarriorAbilitySystemComponent.h"
#include "ActionRPG/AbilitySystem/WarriorGameplayAbility.h"


void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* WarriorASCToGive, int32 ApplyLevel)
{
	check(WarriorASCToGive);

	//�ο� �� �ڵ����� Ȱ��ȭ �Ǵ� �����Ƽ��
	GrantAbilities(ActivateOnGivenAbilities, WarriorASCToGive, ApplyLevel);

	//Ư�� �̺�Ʈ�� �����ϴ� �����Ƽ ���
	GrantAbilities(ReactiveAbilities, WarriorASCToGive, ApplyLevel);
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UWarriorGameplayAbility>>& InAbilitiesToGive, UWarriorAbilitySystemComponent* WarriorASCToGive, int32 ApplyLevel)
{
	if (InAbilitiesToGive.IsEmpty())
	{
		return;
	}

	//GameplayAbility �迭 ��ȸ
	//���� �����Ƽ�� �ο�
	for (const TSubclassOf<UWarriorGameplayAbility>& Ability : InAbilitiesToGive)
	{
		if (!Ability) continue;

		//�����Ƽ ����
		FGameplayAbilitySpec AbilitySpec(Ability);
		//�����Ƽ�� ������
		AbilitySpec.SourceObject = WarriorASCToGive->GetAvatarActor();
		//�����Ƽ�� ���� ����
		AbilitySpec.Level = ApplyLevel;

		//�����Ƽ �ο�
		WarriorASCToGive->GiveAbility(AbilitySpec);
	}
}
