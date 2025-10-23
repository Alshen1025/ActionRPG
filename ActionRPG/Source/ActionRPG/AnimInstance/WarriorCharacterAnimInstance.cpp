// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorCharacterAnimInstance.h"
#include "ActionRPG/Character/ActionRPGBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

void UWarriorCharacterAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<AActionRPGBaseCharacter>(TryGetPawnOwner());
	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}



void UWarriorCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaTime)
{
	if (!OwningCharacter || !OwningMovementComponent) return;

	//Character의 속도
	GroundSpeed = OwningCharacter->GetVelocity().Size2D();

	//가속 여부
	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;

	//회전 계산
	LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwningCharacter->GetVelocity(), OwningCharacter->GetActorRotation());
}