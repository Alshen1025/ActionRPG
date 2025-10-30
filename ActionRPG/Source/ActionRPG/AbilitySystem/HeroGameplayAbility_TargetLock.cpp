// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroGameplayAbility_TargetLock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ActionRPG/Character/WarriorCharacter.h"
#include "ActionRPG/Utils/DebugHelper.h"
#include "Kismet/GameplayStatics.h"
#include "ActionRPG/Widget/WarriorWidgetBase.h"
#include "ActionRPG/Controller/WarriorController.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/SizeBox.h"
#include "ActionRPG/WarriorFunctionLibrary.h"
#include "ActionRPG/Utils/ActionRPGGamePlayTags.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"

void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	TryLockOnTarget();
	InitTargetLockMovement();
	InitTargetLockMappingContext();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ResetTargetLockMovement();
	ResetTargetLockMappingContext();
	CleanUp();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_TargetLock::OnTargetLockTick(float DeltaTime)
{
	//플레이어와 타겟이 Dead상태인지 확인
	if (!CurrentLockedActor ||
		UWarriorFunctionLibrary::NativeDoesActorHaveTag(CurrentLockedActor, WarriorGameplayTags::Public_Status_Dead) ||
		UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Public_Status_Dead)
		)
	{
		CancelTargetLockAbility();
		return;
	}

	//Widget이 타겟을 따라다니도록 Tick으로 업데이트
	SetTargetLockWidgetPosition();

	//플레이어가 타겟을 향해 회전하도록
	const bool bShouldOverrideRotation =
		!UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Player_Status_Rolling)
		&&
		!UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Player_Status_Blocking);

	if (bShouldOverrideRotation)
	{
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(
			GetHeroCharacterFromActorInfo()->GetActorLocation(),
			CurrentLockedActor->GetActorLocation()
		);

		LookAtRot -= FRotator(TargetLockCameraOffsetDistance, 0.f, 0.f);

		const FRotator CurrentControlRot = GetHeroControllerFromActorInfo()->GetControlRotation();
		const FRotator TargetRot = FMath::RInterpTo(CurrentControlRot, LookAtRot, DeltaTime, TargetLockRotationInterpSpeed);

		GetHeroControllerFromActorInfo()->SetControlRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0.f));
		GetHeroCharacterFromActorInfo()->SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
	}
}

void UHeroGameplayAbility_TargetLock::SwitchTarget(const FGameplayTag& InSwitchDirectionTag)
{
	TArray<AActor*> ActorsOnLeft;
	TArray<AActor*> ActorsOnRight;
	AActor* NewTargetToLock = nullptr;

	//후보 Actor검사
	GetAvailableActorsAroundTarget(ActorsOnLeft, ActorsOnRight);

	//태그에 따라서 좌,우 액터 배열에서 가장 가까운 액터 선택
	if (InSwitchDirectionTag == WarriorGameplayTags::Player_Event_SwitchTarget_Left)
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnLeft);
	}
	else
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnRight);
	}


	//선택된 액터를 TargetLock대상으로 지정
	if (NewTargetToLock)
	{
		CurrentLockedActor = NewTargetToLock;
	}
}

void UHeroGameplayAbility_TargetLock::TryLockOnTarget()
{
	//락온 가능한 Actor 식별
	GetAvailableActorsToLock();

	//가능한 Actor가 없으면 return
	if (AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	//가장 가까운 액터 선택
	CurrentLockedActor = GetNearestTargetFromAvailableActors(AvailableActorsToLock);

	if (CurrentLockedActor)
	{
		DrawTargetLockWidget();
	}
	else
	{
		CancelTargetLockAbility();
	}
}

void UHeroGameplayAbility_TargetLock::GetAvailableActorsToLock()
{
	AvailableActorsToLock.Empty();
	TArray<FHitResult> BoxTraceHits;

	//범위 Trace
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetHeroCharacterFromActorInfo(),
		GetHeroCharacterFromActorInfo()->GetActorLocation(), //시작위치
		GetHeroCharacterFromActorInfo()->GetActorLocation() + GetHeroCharacterFromActorInfo()->GetActorForwardVector() * BoxTraceDistance, //종료위치
		TraceBoxSize / 2.f, // 박스의 크기
		GetHeroCharacterFromActorInfo()->GetActorForwardVector().ToOrientationRotator(), //박스 회전 값
		BoxTraceChannel, //감지 오브젝트 유형
		false,
		TArray<AActor*>(), 
		bShowPersistentDebugShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
		BoxTraceHits, //결과 배열
		true //자신 제외
	);

	//TargetLock가능한 Actor들 선별
	for (const FHitResult& TraceHit : BoxTraceHits)
	{
		if (AActor* HitActor = TraceHit.GetActor())
		{
			if (HitActor != GetHeroCharacterFromActorInfo())
			{
				AvailableActorsToLock.AddUnique(HitActor);

				Debug::Print(HitActor->GetActorNameOrLabel());
			}
		}
	}
}

AActor* UHeroGameplayAbility_TargetLock::GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors)
{
	float ClosestDistance = 0.f;
	return UGameplayStatics::FindNearestActor(GetHeroCharacterFromActorInfo()->GetActorLocation(), InAvailableActors, ClosestDistance);
}

void UHeroGameplayAbility_TargetLock::CancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	//초기화
	AvailableActorsToLock.Empty();
	CurrentLockedActor = nullptr;

	if (DrawnTargetLockWidget)
	{
		DrawnTargetLockWidget->RemoveFromParent();
	}
}

void UHeroGameplayAbility_TargetLock::CleanUp()
{
	AvailableActorsToLock.Empty();
	CurrentLockedActor = nullptr;

	if (DrawnTargetLockWidget)
	{
		DrawnTargetLockWidget->RemoveFromParent();
	}

	DrawnTargetLockWidget = nullptr;

	TargetLockWidgetSize = FVector2D::ZeroVector;

	CachedDefaultMaxWalkSpeed = 0.f;
}

void UHeroGameplayAbility_TargetLock::DrawTargetLockWidget()
{
	if (!DrawnTargetLockWidget)
	{
		checkf(TargetLockWidgetClass, TEXT("Forgot to assign a valid widget class in Blueprint"));

		//위젯 생성
		DrawnTargetLockWidget = CreateWidget<UWarriorWidgetBase>(GetHeroControllerFromActorInfo(), TargetLockWidgetClass);

		check(DrawnTargetLockWidget);


		//추가
		DrawnTargetLockWidget->AddToViewport();
	}
}

void UHeroGameplayAbility_TargetLock::SetTargetLockWidgetPosition()
{
	if (!DrawnTargetLockWidget || !CurrentLockedActor)
	{
		CancelTargetLockAbility();
		return;
	}

	//Target액터의 위치를 게임 화면(플레이어 스크린) 기준으로 변환
	FVector2D ScreenPosition;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		GetHeroControllerFromActorInfo(),
		CurrentLockedActor->GetActorLocation(),
		ScreenPosition,
		true
	);

	//WidgetSize 설정
	if (TargetLockWidgetSize == FVector2D::ZeroVector)
	{
		//Widget에 있는 구성요소 순회
		DrawnTargetLockWidget->WidgetTree->ForEachWidget(
			[this](UWidget* FoundWidget) //람다 -> 각 구성요소 대상으로 로직 실행
			{
				if (USizeBox* FoundSizeBox = Cast<USizeBox>(FoundWidget))
				{
					//SizeBox의 값으로 크기 재구성
					TargetLockWidgetSize.X = FoundSizeBox->GetWidthOverride();
					TargetLockWidgetSize.Y = FoundSizeBox->GetHeightOverride();
				}
			}
		);
	}
	//위치 조정
	ScreenPosition -= (TargetLockWidgetSize / 2.f);

	//그 포지션에 설정
	DrawnTargetLockWidget->SetPositionInViewport(ScreenPosition, false);

	//위치 업데이트는 Ability Task에서
}

void UHeroGameplayAbility_TargetLock::InitTargetLockMovement()
{
	CachedDefaultMaxWalkSpeed = GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed;
	GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = TargetLockMaxWalkSpeed;
}

void UHeroGameplayAbility_TargetLock::InitTargetLockMappingContext()
{
	const ULocalPlayer* LocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem)

		Subsystem->AddMappingContext(TargetLockMappingContext, 3);
}

void UHeroGameplayAbility_TargetLock::ResetTargetLockMovement()
{
	if (CachedDefaultMaxWalkSpeed > 0.f)
	{
		GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = CachedDefaultMaxWalkSpeed;
	}
}

void UHeroGameplayAbility_TargetLock::ResetTargetLockMappingContext()
{
	if (!GetHeroControllerFromActorInfo())
	{
		return;
	}

	const ULocalPlayer* LocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem)

		Subsystem->RemoveMappingContext(TargetLockMappingContext);
}

void UHeroGameplayAbility_TargetLock::GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight)
{
	if (!CurrentLockedActor || AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	//플레이어 -> Target 방향 벡터
	const FVector PlayerLocation = GetHeroCharacterFromActorInfo()->GetActorLocation();
	const FVector PlayerToCurrentNormalized = (CurrentLockedActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

	//후보대상 순회
	for (AActor* AvailableActor : AvailableActorsToLock)
	{
		//이미 Target인 액터는 제외
		if (!AvailableActor || AvailableActor == CurrentLockedActor) continue;

		//플레이어 -> 후보 방향벡터
		const FVector PlayerToAvailableNormalized = (AvailableActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

		//플레이어 -> Target 방향 벡터와 플레이어 -> 후보 방향벡터의 외적을 검사.
		const FVector CrossResult = FVector::CrossProduct(PlayerToCurrentNormalized, PlayerToAvailableNormalized);

		//외적 결과로 좌우 판별
		if (CrossResult.Z > 0.f)
		{
			OutActorsOnRight.AddUnique(AvailableActor);
		}
		else
		{
			OutActorsOnLeft.AddUnique(AvailableActor);
		}
	}
}
