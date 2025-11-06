// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ActionRPG/WarriorTypes/WarriorEnumTypes.h"
#include "ActionRPGGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API AActionRPGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AActionRPGGameModeBase();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Settings")
	EWarriorGameDifficulty CurrentGameDifficulty;

public:
	FORCEINLINE EWarriorGameDifficulty GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }
};
