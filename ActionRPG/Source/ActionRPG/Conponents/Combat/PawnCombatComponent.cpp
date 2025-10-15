
#include "PawnCombatComponent.h"

void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag WeaponTag, AWarriorWeaponBase* Weapon, bool bResiterAsEquippedWeapon)
{
	checkf(!CharacterCarriedWeaponMap.Contains(WeaponTag), TEXT("A named named %s has already been added as carried weapon"),*WeaponTag.ToString());
	check(Weapon);

	//가지고 있는 무기 배열에 태그와 무기 추가
	CharacterCarriedWeaponMap.Emplace(WeaponTag, Weapon);

	//bResiterAsEquippedWeapon = 등록한 무기를 바로 장착할 지 여부
	//true면 바로 장착
	if (bResiterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = WeaponTag;
	}
}

AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag WeaponTag) const
{
	if (CharacterCarriedWeaponMap.Contains(WeaponTag))
	{
		//무기 포인터를 가리키는 포인터
		//Find는 포인터를 반환 (AWarriorWeaponBase**랑 마찬가지)
		if (AWarriorWeaponBase* const* FoundWeapon = CharacterCarriedWeaponMap.Find(WeaponTag))
		{
			//우리가 반환하려는 실제 무기 포인터를 얻으려면 역참조 해야함
			return *FoundWeapon;
		}
	}
	return nullptr;
}

AWarriorWeaponBase* UPawnCombatComponent::GetCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid())
	{
		return nullptr;
	}
	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}
