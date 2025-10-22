// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "ActionRPG/Utils/DebugHelper.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

//�θ� Ŭ������ �����ڸ� Super�� ȣ��
//PathFollowingComponent�� ������ ������ Ŭ������ �ƴ϶� UCrowdFollowingComponent�� ����
//DetourCrowdAvoidance�� ����ϱ� ���ؼ� UCrowdFollowingComponent�� ��ü �ʿ�
AEnemyAIController::AEnemyAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		Debug::Print(TEXT("CrowdFollowingComponent valid"), FColor::Green);
	}

	//�ð� ������ ������ ���� ��ü ����
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("EnemySenseConfig_Sight");
	//������ �ĺ��� ��� ����
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	//�Ʊ��� ����
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	//�߸� ��� ����
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	//�þ� �ݰ� ->50����
	AISenseConfig_Sight->SightRadius = 5000.f;
	//����� ��ġ�� �Ÿ�,SightRadius���� �۰� �����ϸ� SightRadius������ �ڵ� ����(SightRadius == LoseSightRadius)
	AISenseConfig_Sight->LoseSightRadius = 0.f;
	//�þ߰� 360��
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f;

	//EnemyPerceptionComponent����
	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("EnemyPerceptionComponent");
	//AISenseConfig_Sight�� ������ ������Ʈ�� ���
	EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	//���� �߿��� ������ �ð����� ����-> �ð��� ���� �켱����
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	//AI�� ����� ���� ���¸� ������Ʈ �� ������ OnEnemyPerceptionUpdatedȣ��
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);

	SetGenericTeamId(FGenericTeamId(1));
}


//                                                                                                �˻� ���
ETeamAttitude::Type AEnemyAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	//�˻� ����� ��Ʈ�ѷ� Ȯ��
	//�Ǿ� �ĺ��� ���� IGenericTeamAgentInterface�� ������ �ִ��� Ȯ��
	const APawn* PawnToCheck = Cast<const APawn>(&Other);
	if (PawnToCheck == nullptr) return ETeamAttitude::Friendly;
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(PawnToCheck->GetController());

	//������ �Ǿ� �ĺ��� �����ϰ� �� ID(AI Enemy)�� ����� ID�� �ٸ��� 
	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() != GetGenericTeamId())
	{
		//�� ����� '��'
		return ETeamAttitude::Hostile;
	}
	//�ƴϸ� �Ʊ�
	return ETeamAttitude::Friendly;
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		//bEnableDetourCrowdAvoidance�� ���� ���� �ùķ��̼� Ȱ��ȭ
		CrowdComp->SetCrowdSimulationState(bEnableDetourCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);

		//AIȸ�� ǰ�� ����
		switch (DetourCrowdAvoidanceQuality)
		{
		case 1: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);    break;
		case 2: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium); break;
		case 3: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);   break;
		case 4: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);   break;
		default:
			break;
		}

		//AI�� �Ҽ� �׷� ����
		CrowdComp->SetAvoidanceGroup(1);
		//���ؾ��� �׷� ����
		CrowdComp->SetGroupsToAvoid(1);
		//���� ��ֹ� ���� ���� ����
		CrowdComp->SetCrowdCollisionQueryRange(CollisionQueryRange);
	}
}

void AEnemyAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed() && Actor)
	{
		if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
		{
			//BT Tree�� TargetActor Key�� ������ Actor����
			BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
		}
	}
}
