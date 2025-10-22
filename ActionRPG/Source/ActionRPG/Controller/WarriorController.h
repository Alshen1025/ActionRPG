// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "WarriorController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API AWarriorController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AWarriorController();

	//IGenericTeamAgentInterface
	virtual FGenericTeamId GetGenericTeamId() const override;

private:
	FGenericTeamId HeroTeamID;
};
