
#include "PawnCombatComponent.h"
#include "ActionRPG/Items/Weapon/WarriorWeaponBase.h"
#include "Components/BoxComponent.h"
#include "ActionRPG/Utils/DebugHelper.h"
void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag WeaponTag, AWarriorWeaponBase* Weapon, bool bResiterAsEquippedWeapon)
{
	checkf(!CharacterCarriedWeaponMap.Contains(WeaponTag), TEXT("A named named %s has already been added as carried weapon"),*WeaponTag.ToString());
	check(Weapon);

	//������ �ִ� ���� �迭�� �±׿� ���� �߰�
	CharacterCarriedWeaponMap.Emplace(WeaponTag, Weapon);

	//Hit Delegate ���
	Weapon->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	Weapon->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);

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

void UPawnCombatComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	//DamageType�� CurrentEquippedWeapon���� Ȯ�� -> �Ǽ��̸� ���� Collision�� Toggle�� �ʿ� ����
	if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		AWarriorWeaponBase* WeaponToToggle = GetCurrentEquippedWeapon();

		check(WeaponToToggle);

		if (bShouldEnable)
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			OverlappedActors.Empty();
		}
	}
}

void UPawnCombatComponent::OnHitTargetActor(AActor* HitActor)
{

}

void UPawnCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{

}
