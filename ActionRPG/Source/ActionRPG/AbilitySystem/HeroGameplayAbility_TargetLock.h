// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WarriorHeroGameplayAbility.h"
#include "HeroGameplayAbility_TargetLock.generated.h"

class UWarriorWidgetBase;
class UInputMappingContext;


UCLASS()
class ACTIONRPG_API UHeroGameplayAbility_TargetLock : public UWarriorHeroGameplayAbility
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION(BlueprintCallable)
	void OnTargetLockTick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void SwitchTarget(const FGameplayTag& InSwitchDirectionTag);

private:
	void TryLockOnTarget();
	void GetAvailableActorsToLock();

	AActor* GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors);

	void CancelTargetLockAbility();

	void CleanUp();

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float BoxTraceDistance = 5000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	FVector TraceBoxSize = FVector(5000.f, 5000.f, 300.f);

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	TArray< TEnumAsByte < EObjectTypeQuery > > BoxTraceChannel;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	bool bShowPersistentDebugShape = false;

	UPROPERTY()
	TArray<AActor*> AvailableActorsToLock;

	UPROPERTY()
	AActor* CurrentLockedActor;

	//widget
	UPROPERTY()
	UWarriorWidgetBase* DrawnTargetLockWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	TSubclassOf<UWarriorWidgetBase> TargetLockWidgetClass;

	void DrawTargetLockWidget();

	UPROPERTY()
	FVector2D TargetLockWidgetSize = FVector2D::ZeroVector;

	void SetTargetLockWidgetPosition();

	//TargetLock
	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float TargetLockRotationInterpSpeed = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float TargetLockMaxWalkSpeed = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float TargetLockCameraOffsetDistance = 20.f;

	void InitTargetLockMovement();
	void InitTargetLockMappingContext();
	void ResetTargetLockMovement();
	void ResetTargetLockMappingContext();

	UPROPERTY()
	float CachedDefaultMaxWalkSpeed = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	UInputMappingContext* TargetLockMappingContext;

	void GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight);
};
