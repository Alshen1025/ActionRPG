// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRPGBaseCharacter.h"
#include "WarriorEnemyCharacter.generated.h"

class UEnemyCombatComponent;
class UEnemyUIComponent;
class UWidgetComponent;
class UBoxComponent;

UCLASS()
class ACTIONRPG_API AWarriorEnemyCharacter : public AActionRPGBaseCharacter
{
	GENERATED_BODY()

public:
	AWarriorEnemyCharacter();

#pragma region PawnCombatInterface
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
#pragma endregion


#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	//PawnUIInterface
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UEnemyUIComponent* GetEnemyUIComponent() const override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UEnemyCombatComponent* EnemyCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UEnemyUIComponent* EnemyUIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UBoxComponent* LeftHandCollisionBox;

	//บฮย๘วา SocketName
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName LeftHandCollisionBoxAttachBoneName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UBoxComponent* RightHandCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName RightHandCollisionBoxAttachBoneName;

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* EnemyHealthWidgetComponent;

	UFUNCTION()
	virtual void OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void InitEnemyStartUpData();

public:
	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; }
	FORCEINLINE UBoxComponent* GetLeftHandCollisionBox() const { return LeftHandCollisionBox; }
	FORCEINLINE UBoxComponent* GetRightHandCollisionBox() const { return RightHandCollisionBox; }

};
