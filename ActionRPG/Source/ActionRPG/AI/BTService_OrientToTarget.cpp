// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_OrientToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"


UBTService_OrientToTarget::UBTService_OrientToTarget()
{
	//Sercice Node일름
	NodeName = TEXT("Native Orient Rotation To Target Actor");

	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	//회전 보간 속도
	RotationInterpSpeed = 5.f;
	//서비스 노드의 실행 주기 -> 매 프레임 마다 실행
	Interval = 0.f;
	
	RandomDeviation = 0.f;

	//InTargetActorKey가 오직 Actor타입만 저장하도록
	InTargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey), AActor::StaticClass());
}

void UBTService_OrientToTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	//BT에 연결된 블랙보드 가져오기
	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		//블랙보드에서 키 가져와서 저장 InTargetActorKey->에디터에서 설정 가능 변수
		InTargetActorKey.ResolveSelectedKey(*BBAsset);
	}
}

FString UBTService_OrientToTarget::GetStaticDescription() const
{
	//서비스 노드의 역할 문자열 반환
	const FString KeyDescription = InTargetActorKey.SelectedKeyName.ToString();

	return FString::Printf(TEXT("Orient rotation to %s Key %s"), *KeyDescription, *GetStaticServiceDescription());
}

//회전을 Task가 아니라 Service에서 구현한 이유
//Task노드가 실행되는 동안 갱신되어야 하는 보조 행동들은 Service로 구현해야 병렬처리 가능
void UBTService_OrientToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//InTargetActorKey의 Actor가져오기ㅈ
	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ActorObject);

	//자기 자신
	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();


	//자기 자신과 타깃이 모두 유효하면
	if (OwningPawn && TargetActor)
	{
		//본인 위치에서 타겟의 위치를 바라보는 목표 회전 값 계산
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(OwningPawn->GetActorLocation(), TargetActor->GetActorLocation());
		//회전 보간
		const FRotator TargetRot = FMath::RInterpTo(OwningPawn->GetActorRotation(), LookAtRot, DeltaSeconds, RotationInterpSpeed);

		//보간 값 적용
		OwningPawn->SetActorRotation(TargetRot);
	}
}
