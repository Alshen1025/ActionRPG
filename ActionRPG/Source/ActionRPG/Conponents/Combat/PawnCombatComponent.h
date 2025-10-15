// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRPG/Conponents/PawnExtensionComponentBase.h"
#include "GameplayTagContainer.h"
#include "PawnCombatComponent.generated.h"

class AWarriorWeaponBase;

UCLASS()
class ACTIONRPG_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()
	
#pragma region Weapon

public:
	//���� ���
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	void RegisterSpawnedWeapon(FGameplayTag WeaponTag, AWarriorWeaponBase* Weapon, bool bResiterAsEquippedWeapon = false);

	//������ �ִ� ���⸦ �±׸� �̿��� ��������
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag WeaponTag) const;

	//���� �������� ������ �±�
	UPROPERTY(BlueprintReadWrite, Category = "Warrior|Combat")
	FGameplayTag CurrentEquippedWeaponTag;

	//���� �������� ���� ��������
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorWeaponBase* GetCurrentEquippedWeapon() const;

private:
	//������ �ִ� ����
	TMap<FGameplayTag, AWarriorWeaponBase*> CharacterCarriedWeaponMap;

#pragma endregion
};
