// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRPGBaseCharacter.h"
#include "WarriorEnemyCharacter.generated.h"

class UEnemyCombatComponent;

UCLASS()
class ACTIONRPG_API AWarriorEnemyCharacter : public AActionRPGBaseCharacter
{
	GENERATED_BODY()

public:
	AWarriorEnemyCharacter();

#pragma region PawnCombatInterface
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
#pragma endregion

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UEnemyCombatComponent* EnemyCombatComponent;

	virtual void PossessedBy(AController* NewController) override;

private:
	void InitEnemyStartUpData();

public:
	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; }
	

};
