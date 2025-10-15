// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorAbilitySystemComponent.h"

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
