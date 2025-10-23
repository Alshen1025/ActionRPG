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

	//Character�� �ӵ�
	GroundSpeed = OwningCharacter->GetVelocity().Size2D();

	//���� ����
	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;

	//ȸ�� ���
	LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwningCharacter->GetVelocity(), OwningCharacter->GetActorRotation());
}