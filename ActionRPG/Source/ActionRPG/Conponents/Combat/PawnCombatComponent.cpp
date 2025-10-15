
#include "PawnCombatComponent.h"

void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag WeaponTag, AWarriorWeaponBase* Weapon, bool bResiterAsEquippedWeapon)
{
	checkf(!CharacterCarriedWeaponMap.Contains(WeaponTag), TEXT("A named named %s has already been added as carried weapon"),*WeaponTag.ToString());
	check(Weapon);

	//������ �ִ� ���� �迭�� �±׿� ���� �߰�
	CharacterCarriedWeaponMap.Emplace(WeaponTag, Weapon);

	//bResiterAsEquippedWeapon = ����� ���⸦ �ٷ� ������ �� ����
	//true�� �ٷ� ����
	if (bResiterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = WeaponTag;
	}
}

AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag WeaponTag) const
{
	if (CharacterCarriedWeaponMap.Contains(WeaponTag))
	{
		//���� �����͸� ����Ű�� ������
		//Find�� �����͸� ��ȯ (AWarriorWeaponBase**�� ��������)
		if (AWarriorWeaponBase* const* FoundWeapon = CharacterCarriedWeaponMap.Find(WeaponTag))
		{
			//�츮�� ��ȯ�Ϸ��� ���� ���� �����͸� �������� ������ �ؾ���
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
