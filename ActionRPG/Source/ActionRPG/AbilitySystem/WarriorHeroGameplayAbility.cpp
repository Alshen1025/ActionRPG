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

	//GameplayEffect의 context생성
	FGameplayEffectContextHandle ContextHandle = GetWarriorAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	//Effect가 실행중인 어빌리티(this)로부터 발생되었음 명시
	ContextHandle.SetAbility(this);
	//Effect를 발생시킨 구체적인 오브젝트 -> 어빌리티를 사용하는 캐릭터
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	//효과를 유발한 주체
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle EffectSpecHandle = GetWarriorAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);
	//SetByCallerMagnitude는 C++코드에서 실시간으로 계산된 값을 전달할 때 사용
	//SetSetByCallerMagnitude는 블루프린트에서 사용

	//Public_SetByCaller_BaseDamage태그에 WeaponBaseDamage(기본 데미지)값 저장
	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		WarriorGameplayTags::Public_SetByCaller_BaseDamage,
		InWeaponBaseDamage
	);

	//AttckType과 CureentComboCount를 넘김, 공격 타입과 콤보수에 따라 데미지 적용에 차이를 둘 수도 있음
	if (InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InCurrentComboCount);
	}

	//SpecHandle반환
	return EffectSpecHandle;
}
