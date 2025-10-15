// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRPGAnimInstanceBase.h"
#include "WarriorLinkedAnimLayer.generated.h"

class UWarriorCharacterAnimInstance;
/**
 * 
 */
UCLASS()
class ACTIONRPG_API UWarriorLinkedAnimLayer : public UActionRPGAnimInstanceBase
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UWarriorCharacterAnimInstance* GetHeroAnimInstance() const;
};
