// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorHeroGameplayAbility.h"
#include "ActionRPG/Character/WarriorCharacter.h"
#include "ActionRPG/Controller/WarriorController.h"
#include "ActionRPG/AbilitySystem/WarriorAbilitySystemComponent.h"
#include "ActionRPG/Utils/ActionRPGGamePlayTags.h"

AWarriorCharacter* UWarriorHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
	if (!CachedWarriorHeroCharacter.IsValid())
	{
		CachedWarriorHeroCharacter = Cast<AWarriorCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedWarriorHeroCharacter.IsValid() ? CachedWarriorHeroCharacter.Get() : nullptr;
}

AWarriorController* UWarriorHeroGameplayAbility::GetHeroControllerFromActorInfo()
{
	if (!CachedWarriorHeroController.IsValid())
	{
		CachedWarriorHeroController = Cast<AWarriorController>(CurrentActorInfo->PlayerController);
	}
	return CachedWarriorHeroController.IsValid() ? CachedWarriorHeroController.Get() : nullptr;
}

UHeroCombatComponent* UWarriorHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
	return GetHeroCharacterFromActorInfo()->GetHeroCombatComponent();
}

FGameplayEffectSpecHandle UWarriorHeroGameplayAbility::MakeHeroDamageEffectHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InCurrentComboCount)
{
	check(EffectClass);

	//GameplayEffect�� context����
	FGameplayEffectContextHandle ContextHandle = GetWarriorAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	//Effect�� �������� �����Ƽ(this)�κ��� �߻��Ǿ��� ���
	ContextHandle.SetAbility(this);
	//Effect�� �߻���Ų ��ü���� ������Ʈ -> �����Ƽ�� ����ϴ� ĳ����
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	//ȿ���� ������ ��ü
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle EffectSpecHandle = GetWarriorAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);
	//SetByCallerMagnitude�� C++�ڵ忡�� �ǽð����� ���� ���� ������ �� ���
	//SetSetByCallerMagnitude�� �������Ʈ���� ���

	//Public_SetByCaller_BaseDamage�±׿� WeaponBaseDamage(�⺻ ������)�� ����
	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		WarriorGameplayTags::Public_SetByCaller_BaseDamage,
		InWeaponBaseDamage
	);

	//AttckType�� CureentComboCount�� �ѱ�, ���� Ÿ�԰� �޺����� ���� ������ ���뿡 ���̸� �� ���� ����
	if (InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InCurrentComboCount);
	}

	//SpecHandle��ȯ
	return EffectSpecHandle;
}
