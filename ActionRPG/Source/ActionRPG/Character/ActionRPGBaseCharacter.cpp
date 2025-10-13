// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRPGBaseCharacter.h"

// Sets default values
AActionRPGBaseCharacter::AActionRPGBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;
}

