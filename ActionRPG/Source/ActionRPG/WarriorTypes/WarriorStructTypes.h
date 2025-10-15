// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "WarriorStructTypes.generated.h"


class UWarriorLinkedAnimLayer;


USTRUCT(BlueprintType)
struct FWarriorHeroWeaponData
{
	GENERATED_BODY()

	//�����ؾ��� Layer���� ����ü
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWarriorLinkedAnimLayer> WeaponAnimLayerToLink;
};