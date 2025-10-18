// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRPG/Conponents/PawnExtensionComponentBase.h"
#include "GameplayTagContainer.h"
#include "PawnCombatComponent.generated.h"

class AWarriorWeaponBase;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
	LeftHand,
	RightHand
};

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

	//������ Collision Toggle
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	void ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType = EToggleDamageType::CurrentEquippedWeapon);

	//Hit Delegate
	virtual void OnHitTargetActor(AActor* HitActor);

	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor);

protected:
	//���⿡ ���� Actor�迭
	TArray<AActor*> OverlappedActors;

private:
	//������ �ִ� ����
	TMap<FGameplayTag, AWarriorWeaponBase*> CharacterCarriedWeaponMap;

#pragma endregion
};
