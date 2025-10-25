// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCombatComponent.h"
#include "ActionRPG/Utils/DebugHelper.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ActionRPG/Utils/ActionRPGGamePlayTags.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor))
	{
		//���� ó���� Actor�� (OverlappedActors) return
		Debug::Print(GetOwningPawn()->GetActorNameOrLabel() + TEXT(" is hitting ") + HitActor->GetActorNameOrLabel());
		return;
	}

	OverlappedActors.AddUnique(HitActor);

	//TODO:: Implement block check
	bool bIsValidBlock = false;

	//�÷��̾ ����ϰ� �ִ°�
	const bool bIsPlayerBlocking = false;
	const bool bIsMyAttackUnblockable = false;

	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		//�÷��̾� ��� �� ����
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;

	if (bIsValidBlock)
	{
		//��� ����
	}
	else
	{
		//������� �ʰų� ��� ���н� Event����
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			WarriorGameplayTags::Public_Event_MeleeHit,
			EventData
		);
	}
}
