// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRPGAnimInstanceBase.h"
#include "ActionRPG/WarriorFunctionLibrary.h"

bool UActionRPGAnimInstanceBase::DoesOwnerHaveTag(FGameplayTag TagToCheck) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		return UWarriorFunctionLibrary::NativeDoesActorHaveTag(OwningPawn, TagToCheck);
	}

	return false;
}
