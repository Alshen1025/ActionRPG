// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RotateToFaceTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_RotateToFaceTarget::UBTTask_RotateToFaceTarget()
{
	NodeName = TEXT("Native Rotate to Face Target Actor");
	AnglePrecision = 10.f;
	RotationInterpSpeed = 5.f;

	bNotifyTick = true;
	bNotifyTaskFinished = true;

	//Task�� AI���� �������� ����
	//false�� �����ϰ� GetInstanceMemorySize�� Ŀ���� �޸� ���
	bCreateNodeInstance = false;

	INIT_TASK_NODE_NOTIFY_FLAGS();

	InTargetToFaceKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetToFaceKey), AActor::StaticClass());
}

void UBTTask_RotateToFaceTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		InTargetToFaceKey.ResolveSelectedKey(*BBAsset);
	}
}

//���� ������ ���� �ʿ��� �޸��� ũ�⸦ ��ȯ
uint16 UBTTask_RotateToFaceTarget::GetInstanceMemorySize() const
{
	// FRotateToFaceTargetTaskMemory ����ü�� ũ�⸦ ��ȯ�մϴ�.
	return sizeof(FRotateToFaceTargetTaskMemory);
}

FString UBTTask_RotateToFaceTarget::GetStaticDescription() const
{
	const FString KeyDescription = InTargetToFaceKey.SelectedKeyName.ToString();
	return FString::Printf(TEXT("Smoothly rotates to face %s Key until the angle precision: %s is reached"), *KeyDescription, *FString::SanitizeFloat(AnglePrecision));
}

//Task�� ó�� ����� �� ȣ��
EBTNodeResult::Type UBTTask_RotateToFaceTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Target�� �ڱ� �ڽ� ��������
	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetToFaceKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ActorObject);
	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

	//GetInstanceMemorySize�� �Ҵ�� �½�ũ ���� �޸� ��� ��������
	FRotateToFaceTargetTaskMemory* Memory = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemory>(NodeMemory);
	check(Memory);

	//�޸𸮿� ����
	Memory->OwningPawn = OwningPawn;
	Memory->TargetActor = TargetActor;

	if (!Memory->IsValid())
	{
		return EBTNodeResult::Failed;
	}

	//�̹� Ÿ���� �ٶ󺸰� ������
	if (HasReachedAnglePrecision(OwningPawn, TargetActor))
	{
		//�޸� �ʱ�ȭ �� ���� ó��
		Memory->Reset();
		return EBTNodeResult::Succeeded;
	}
	// Ÿ���� �ٶ󺸰� ���� �ʴٸ� ������
	return EBTNodeResult::InProgress;
}

void UBTTask_RotateToFaceTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FRotateToFaceTargetTaskMemory* Memory = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemory>(NodeMemory);

	//�޸𸮰� ��ȿ���� ������
	if (!Memory->IsValid())
	{
		//����ó��
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	//�̹� Ÿ���� �ٶ󺸰� �ִ� ���
	if (HasReachedAnglePrecision(Memory->OwningPawn.Get(), Memory->TargetActor.Get()))
	{
		Memory->Reset();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		//ȸ��ó��
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(Memory->OwningPawn->GetActorLocation(), Memory->TargetActor->GetActorLocation());
		const FRotator TargetRot = FMath::RInterpTo(Memory->OwningPawn->GetActorRotation(), LookAtRot, DeltaSeconds, RotationInterpSpeed);

		Memory->OwningPawn->SetActorRotation(TargetRot);
	}
}

//Ÿ���� �ٶ󺸰� �ִ��� Ȯ��
bool UBTTask_RotateToFaceTarget::HasReachedAnglePrecision(APawn* QueryPawn, AActor* TargetActor) const
{
	//Owner�� �ٶ󺸴� ���� ����
	const FVector OwnerForward = QueryPawn->GetActorForwardVector();
	//Owner���� Target���� ���ϴ� ���⺤��
	const FVector OwnerToTargetNormalized = (TargetActor->GetActorLocation() - QueryPawn->GetActorLocation()).GetSafeNormal();

	//���� ���
	const float DotResult = FVector::DotProduct(OwnerForward, OwnerToTargetNormalized);
	const float AngleDiff = UKismetMathLibrary::DegAcos(DotResult);

	//�������̰� ��� ���� ���� true��ȯ
	return AngleDiff <= AnglePrecision;
}
