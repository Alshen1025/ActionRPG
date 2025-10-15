// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "WarriorStructTypes.generated.h"


class UWarriorLinkedAnimLayer;


USTRUCT(BlueprintType)
struct FWarriorHeroWeaponData
{
	GENERATED_BODY()

	//연결해야할 Layer저장 구조체
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWarriorLinkedAnimLayer> WeaponAnimLayerToLink;
};