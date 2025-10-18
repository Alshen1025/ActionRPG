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
	//무기 등록
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	void RegisterSpawnedWeapon(FGameplayTag WeaponTag, AWarriorWeaponBase* Weapon, bool bResiterAsEquippedWeapon = false);

	//가지고 있는 무기를 태그를 이용해 가져오기
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag WeaponTag) const;

	//현재 장착중인 무기의 태그
	UPROPERTY(BlueprintReadWrite, Category = "Warrior|Combat")
	FGameplayTag CurrentEquippedWeaponTag;

	//현재 장착중인 무기 가져오기
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorWeaponBase* GetCurrentEquippedWeapon() const;

	//무기의 Collision Toggle
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	void ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType = EToggleDamageType::CurrentEquippedWeapon);

	//Hit Delegate
	virtual void OnHitTargetActor(AActor* HitActor);

	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor);

protected:
	//무기에 맞은 Actor배열
	TArray<AActor*> OverlappedActors;

private:
	//가지고 있는 무기
	TMap<FGameplayTag, AWarriorWeaponBase*> CharacterCarriedWeaponMap;

#pragma endregion
};
