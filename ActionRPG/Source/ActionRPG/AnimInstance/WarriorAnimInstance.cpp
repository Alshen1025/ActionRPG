// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorAnimInstance.h"
#include "ActionRPG/Character/WarriorCharacter.h"


void UWarriorAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwningCharacter)
	{
		OwningHeroCharacter = Cast<AWarriorCharacter>(OwningCharacter);
	}
}

void UWarriorAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (bHasAcceleration)
	{
		IdleElpasedTime = 0.f;
		bEnterRelaxState = false;
	}
	else
	{
		//Idle 상태에서 오랫동안 행동하지 않으면  Relax애니메이션 출력
		IdleElpasedTime += DeltaSeconds;
		bEnterRelaxState = (IdleElpasedTime >= EnterRelaxtStateThreshold);
	}
}