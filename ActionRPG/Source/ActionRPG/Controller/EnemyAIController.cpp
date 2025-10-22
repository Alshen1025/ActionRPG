// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "ActionRPG/Utils/DebugHelper.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

//부모 클래스의 생성자를 Super로 호출
//PathFollowingComponent를 기존에 설정된 클래스가 아니라 UCrowdFollowingComponent로 설정
//DetourCrowdAvoidance를 사용하기 위해서 UCrowdFollowingComponent로 교체 필요
AEnemyAIController::AEnemyAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		Debug::Print(TEXT("CrowdFollowingComponent valid"), FColor::Green);
	}

	//시각 센서의 설정을 담을 객체 생성
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("EnemySenseConfig_Sight");
	//적으로 식별된 대상만 감지
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	//아군은 무시
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	//중립 대상도 무시
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	//시야 반경 ->50미터
	AISenseConfig_Sight->SightRadius = 5000.f;
	//대상을 놓치는 거리,SightRadius보다 작게 설정하면 SightRadius값으로 자동 설정(SightRadius == LoseSightRadius)
	AISenseConfig_Sight->LoseSightRadius = 0.f;
	//시야각 360도
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f;

	//EnemyPerceptionComponent생성
	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("EnemyPerceptionComponent");
	//AISenseConfig_Sight의 설정을 컴포넌트에 등록
	EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	//가장 중요한 감각을 시각으로 설정-> 시각을 가장 우선시함
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	//AI가 대상의 인지 상태를 업데이트 할 때마다 OnEnemyPerceptionUpdated호출
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);

	SetGenericTeamId(FGenericTeamId(1));
}


//                                                                                                검사 대상
ETeamAttitude::Type AEnemyAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	//검사 대상의 컨트롤러 확인
	//피아 식별을 위한 IGenericTeamAgentInterface를 가지고 있는지 확인
	const APawn* PawnToCheck = Cast<const APawn>(&Other);
	if (PawnToCheck == nullptr) return ETeamAttitude::Friendly;
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(PawnToCheck->GetController());

	//상대방의 피아 식별이 가능하고 내 ID(AI Enemy)가 상대의 ID와 다르면 
	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() != GetGenericTeamId())
	{
		//그 대상은 '적'
		return ETeamAttitude::Hostile;
	}
	//아니면 아군
	return ETeamAttitude::Friendly;
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		//bEnableDetourCrowdAvoidance에 따라 군중 시뮬레이션 활성화
		CrowdComp->SetCrowdSimulationState(bEnableDetourCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);

		//AI회피 품질 설정
		switch (DetourCrowdAvoidanceQuality)
		{
		case 1: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);    break;
		case 2: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium); break;
		case 3: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);   break;
		case 4: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);   break;
		default:
			break;
		}

		//AI의 소속 그룹 설정
		CrowdComp->SetAvoidanceGroup(1);
		//피해야할 그룹 설정
		CrowdComp->SetGroupsToAvoid(1);
		//정적 장애물 감지 범위 설정
		CrowdComp->SetCrowdCollisionQueryRange(CollisionQueryRange);
	}
}

void AEnemyAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed() && Actor)
	{
		if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
		{
			//BT Tree의 TargetActor Key에 감지된 Actor전달
			BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
		}
	}
}
