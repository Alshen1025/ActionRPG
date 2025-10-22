// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;


UCLASS()
class ACTIONRPG_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	// FObjectInitializer->언리얼 엔진이 UObject를 생성하고 초기화할 때 사용하는 구조체
	// 기본 서브 오브젝트(DefaultSubObject) 교체에 주로 사용
	AEnemyAIController(const FObjectInitializer& ObjectInitializer);


	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	virtual void BeginPlay() override;

	//AI의 모든 감각을 관리하는 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* EnemyPerceptionComponent;

	//AI 감각의 설정값 Sight->시각
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAISenseConfig_Sight* AISenseConfig_Sight;

	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
	//DetourCrowd사용 여부
	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config")
	bool bEnableDetourCrowdAvoidance = true;

	//AI회피 품질                                                                                  bEnableDetourCrowdAvoidance 활성화 시에만 편집 가능            입력 범위 제한 
	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance", UIMin = "1", UIMax = "4"))
	int32 DetourCrowdAvoidanceQuality = 4;

	//장애물을 감지할 거리																			bEnableDetourCrowdAvoidance 활성화 시에만 편집 가능
	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance"))
	float CollisionQueryRange = 600.f;
};
