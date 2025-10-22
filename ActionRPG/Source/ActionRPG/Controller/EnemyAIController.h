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
	// FObjectInitializer->�𸮾� ������ UObject�� �����ϰ� �ʱ�ȭ�� �� ����ϴ� ����ü
	// �⺻ ���� ������Ʈ(DefaultSubObject) ��ü�� �ַ� ���
	AEnemyAIController(const FObjectInitializer& ObjectInitializer);


	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	virtual void BeginPlay() override;

	//AI�� ��� ������ �����ϴ� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* EnemyPerceptionComponent;

	//AI ������ ������ Sight->�ð�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAISenseConfig_Sight* AISenseConfig_Sight;

	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
	//DetourCrowd��� ����
	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config")
	bool bEnableDetourCrowdAvoidance = true;

	//AIȸ�� ǰ��                                                                                  bEnableDetourCrowdAvoidance Ȱ��ȭ �ÿ��� ���� ����            �Է� ���� ���� 
	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance", UIMin = "1", UIMax = "4"))
	int32 DetourCrowdAvoidanceQuality = 4;

	//��ֹ��� ������ �Ÿ�																			bEnableDetourCrowdAvoidance Ȱ��ȭ �ÿ��� ���� ����
	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance"))
	float CollisionQueryRange = 600.f;
};
