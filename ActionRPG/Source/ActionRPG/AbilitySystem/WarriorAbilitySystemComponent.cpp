// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorAbilitySystemComponent.h"
#include "ActionRPG/AbilitySystem/WarriorHeroGameplayAbility.h"

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

void UWarriorAbilitySystemComponent::GrantHeroWeaponAbilities(const TArray<FWarriorHeroAbilitySet>& InDefaultWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (InDefaultWeaponAbilities.IsEmpty())
	{
		return;
	}

	//어빌리티 구조체 순회
	for (const FWarriorHeroAbilitySet& AbilitySet : InDefaultWeaponAbilities)
	{
		if (!AbilitySet.IsValid()) continue;

		//설정
		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

		//AddUnique->중복 없이
		//추후에 무기를 해제할 때를 대비해 부여한 무기 어빌리티 저장
		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}
}

void UWarriorAbilitySystemComponent::RemovedGrantedHeroWeaponAbilities(UPARAM(ref)TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove)
{
	if (InSpecHandlesToRemove.IsEmpty())
	{
		return;
	}
	//부여했던 어빌리티 제거
	for (const FGameplayAbilitySpecHandle& SpecHandle : InSpecHandlesToRemove)
	{
		if (SpecHandle.IsValid())
		{
			ClearAbility(SpecHandle);
		}
	}
	InSpecHandlesToRemove.Empty();
}

//여러개의 어빌리티 중 랜덤으로 하나 활성화
bool UWarriorAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{
	//태그 유효성 검사
	check(AbilityTagToActivate.IsValid());

	//Ability Spec 포인터 저장을 위한 배열
	TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;
	//ASC가 가진 어빌리티 중에서 활성화 가능하고  AbilityTagToActivate를 모두 포함하는 Spec을 찾아 배열에 채워넣기
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(), FoundAbilitySpecs);

	//태그와 일치하는 어빌리티를 찾았다면
	if (!FoundAbilitySpecs.IsEmpty())
	{
		//일치하는 어빌리티가 여러 개 라면 무작위 인덱스 선택
		const int32 RandomAbilityIndex = FMath::RandRange(0, FoundAbilitySpecs.Num() - 1);
		//그 인덱스를 이용해 Spec하나 고르기
		FGameplayAbilitySpec* SpecToActivate = FoundAbilitySpecs[RandomAbilityIndex];

		check(SpecToActivate);

		//고른 어빌리티가 활성화 되지 않았다면
		if (!SpecToActivate->IsActive())
		{
			//활성화 시도 후 결과 리턴
			return TryActivateAbility(SpecToActivate->Handle);
		}
	}
	return false;
}
