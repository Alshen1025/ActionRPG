// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroCombatComponent.h"
#include "ActionRPG/Items/Weapon/WarriorWeapon.h"
#include "ActionRPG/Utils/DebugHelper.h"
#include "ActionRPG/Utils/ActionRPGGamePlayTags.h"
#include "AbilitySystemBlueprintLibrary.h"

AWarriorWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(FGameplayTag WeaponTag) const
{
	return Cast<AWarriorWeapon>(GetCharacterCarriedWeaponByTag(WeaponTag));
}

void UHeroCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	Debug::Print(GetOwningPawn()->GetActorNameOrLabel() + TEXT(" hit ") + HitActor->GetActorNameOrLabel(), FColor::Green);

	//이미 피격되었는지 확인
	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}

	OverlappedActors.AddUnique(HitActor);

	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		WarriorGameplayTags::Public_Event_MeleeHit,
		Data
	);
}

void UHeroCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	Debug::Print(GetOwningPawn()->GetActorNameOrLabel() + TEXT("'s weapon pulled from ") + InteractedActor->GetActorNameOrLabel(), FColor::Red);
}
