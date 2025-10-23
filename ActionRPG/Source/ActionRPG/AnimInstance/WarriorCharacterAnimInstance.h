

#pragma once

#include "CoreMinimal.h"
#include "ActionRPGAnimInstanceBase.h"
#include "WarriorCharacterAnimInstance.generated.h"

class AActionRPGBaseCharacter;
class UCharacterMovementComponent;


UCLASS()
class ACTIONRPG_API UWarriorCharacterAnimInstance : public UActionRPGAnimInstanceBase
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds ) override;

protected:
	UPROPERTY()
	AActionRPGBaseCharacter* OwningCharacter;

	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float LocomotionDirection;
};
