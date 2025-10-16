// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorAbilitySystemComponent.h"
#include "ActionRPG/AbilitySystem/WarriorGameplayAbility.h"

void UWarriorAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	//Ȱ��ȭ ������ �����Ƽ ��ȸ
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		//InputTag�� �ش��ϴ� �����Ƽ�� �ƴϸ� Continue
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) continue;
		//������ Ȱ��ȭ
		TryActivateAbility(AbilitySpec.Handle);
	}
}

void UWarriorAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InputTag)
{

}

void UWarriorAbilitySystemComponent::GrantHeroWeaponAbilities(const TArray<FWarriorHeroAbilitySet>& InDefaultWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (InDefaultWeaponAbilities.IsEmpty())
	{
		return;
	}

	//�����Ƽ ����ü ��ȸ
	for (const FWarriorHeroAbilitySet& AbilitySet : InDefaultWeaponAbilities)
	{
		if (!AbilitySet.IsValid()) continue;

		//����
		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

		//AddUnique->�ߺ� ����
		//���Ŀ� ���⸦ ������ ���� ����� �ο��� ���� �����Ƽ ����
		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}
}

void UWarriorAbilitySystemComponent::RemovedGrantedHeroWeaponAbilities(UPARAM(ref)TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove)
{
	if (InSpecHandlesToRemove.IsEmpty())
	{
		return;
	}
	//�ο��ߴ� �����Ƽ ����
	for (const FGameplayAbilitySpecHandle& SpecHandle : InSpecHandlesToRemove)
	{
		if (SpecHandle.IsValid())
		{
			ClearAbility(SpecHandle);
		}
	}
	InSpecHandlesToRemove.Empty();
}
