// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorLinkedAnimLayer.h"
#include "ActionRPG/AnimInstance/WarriorCharacterAnimInstance.h"

UWarriorCharacterAnimInstance* UWarriorLinkedAnimLayer::GetHeroAnimInstance() const
{
	return Cast<UWarriorCharacterAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
