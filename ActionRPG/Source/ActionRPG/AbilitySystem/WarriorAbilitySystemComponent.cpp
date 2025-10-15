// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorAbilitySystemComponent.h"

void UWarriorAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	//활성화 가능한 어빌리티 순회
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		//InputTag에 해당하는 어빌리티가 아니면 Continue
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) continue;
		//맞으면 활성화
		TryActivateAbility(AbilitySpec.Handle);
	}
}

void UWarriorAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InputTag)
{

}
