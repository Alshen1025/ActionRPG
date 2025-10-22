// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_OrientToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"


UBTService_OrientToTarget::UBTService_OrientToTarget()
{
	//Sercice Node�ϸ�
	NodeName = TEXT("Native Orient Rotation To Target Actor");

	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	//ȸ�� ���� �ӵ�
	RotationInterpSpeed = 5.f;
	//���� ����� ���� �ֱ� -> �� ������ ���� ����
	Interval = 0.f;
	
	RandomDeviation = 0.f;

	//InTargetActorKey�� ���� ActorŸ�Ը� �����ϵ���
	InTargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey), AActor::StaticClass());
}

void UBTService_OrientToTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	//BT�� ����� ������ ��������
	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		//�����忡�� Ű �����ͼ� ���� InTargetActorKey->�����Ϳ��� ���� ���� ����
		InTargetActorKey.ResolveSelectedKey(*BBAsset);
	}
}

FString UBTService_OrientToTarget::GetStaticDescription() const
{
	//���� ����� ���� ���ڿ� ��ȯ
	const FString KeyDescription = InTargetActorKey.SelectedKeyName.ToString();

	return FString::Printf(TEXT("Orient rotation to %s Key %s"), *KeyDescription, *GetStaticServiceDescription());
}

//ȸ���� Task�� �ƴ϶� Service���� ������ ����
//Task��尡 ����Ǵ� ���� ���ŵǾ�� �ϴ� ���� �ൿ���� Service�� �����ؾ� ����ó�� ����
void UBTService_OrientToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//InTargetActorKey�� Actor�������⤸
	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ActorObject);

	//�ڱ� �ڽ�
	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();


	//�ڱ� �ڽŰ� Ÿ���� ��� ��ȿ�ϸ�
	if (OwningPawn && TargetActor)
	{
		//���� ��ġ���� Ÿ���� ��ġ�� �ٶ󺸴� ��ǥ ȸ�� �� ���
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(OwningPawn->GetActorLocation(), TargetActor->GetActorLocation());
		//ȸ�� ����
		const FRotator TargetRot = FMath::RInterpTo(OwningPawn->GetActorRotation(), LookAtRot, DeltaSeconds, RotationInterpSpeed);

		//���� �� ����
		OwningPawn->SetActorRotation(TargetRot);
	}
}
