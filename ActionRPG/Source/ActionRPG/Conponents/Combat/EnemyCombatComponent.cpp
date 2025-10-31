// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCombatComponent.h"
#include "ActionRPG/Utils/DebugHelper.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ActionRPG/Utils/ActionRPGGamePlayTags.h"
#include "ActionRPG/WarriorFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "ActionRPG/Character/WarriorEnemyCharacter.h"
void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor))
	{
		//공격 처리한 Actor면 (OverlappedActors) return
		Debug::Print(GetOwningPawn()->GetActorNameOrLabel() + TEXT(" is hitting ") + HitActor->GetActorNameOrLabel());
		return;
	}

	OverlappedActors.AddUnique(HitActor);

	//TODO:: Implement block check
	bool bIsValidBlock = false;

	//플레이어가 방어하고 있는가
	const bool bIsPlayerBlocking = UWarriorFunctionLibrary::NativeDoesActorHaveTag(HitActor, WarriorGameplayTags::Player_Status_Blocking);
	const bool bIsMyAttackUnblockable = false;

	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		//방어 결과(마주보고 있는지)
		bIsValidBlock = UWarriorFunctionLibrary::IsValidBlock(GetOwningPawn(), HitActor);
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;

	if (bIsValidBlock)
	{
		//방어 성공시 GameplayEvent전송
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitActor,
			WarriorGameplayTags::Player_Event_SuccessfulBlock,
			EventData
		);
	}
	else
	{
		//방어하지 않거나 방어 실패시 Event전송
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			WarriorGameplayTags::Public_Event_MeleeHit,
			EventData
		);
	}
}

void UEnemyCombatComponent::ToggleBodyCollsionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	//형변환
	AWarriorEnemyCharacter* OwningEnemyCharacter = GetOwningPawn<AWarriorEnemyCharacter>();

	check(OwningEnemyCharacter);

	//양손 CollisionBox얻어오기
	UBoxComponent* LeftHandCollisionBox = OwningEnemyCharacter->GetLeftHandCollisionBox();
	UBoxComponent* RightHandCollisionBox = OwningEnemyCharacter->GetRightHandCollisionBox();

	check(LeftHandCollisionBox && RightHandCollisionBox);

	//CollisionToggle
	switch (ToggleDamageType)
	{
	case EToggleDamageType::LeftHand:
		LeftHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;

	case EToggleDamageType::RightHand:
		RightHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;

	default:
		break;
	}

	//
	if (!bShouldEnable)
	{
		OverlappedActors.Empty();
	}
}
