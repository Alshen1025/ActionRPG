// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask_WaitSpawnEnemies.h"
#include "AbilitySystemComponent.h"
#include "Engine/AssetManager.h"
#include "NavigationSystem.h"
#include "ActionRPG/Character/WarriorEnemyCharacter.h"


//BP 노드 생성
UAbilityTask_WaitSpawnEnemies* UAbilityTask_WaitSpawnEnemies::WaitSpawnEnemies(UGameplayAbility* OwningAbility, FGameplayTag EventTag, TSoftClassPtr<AWarriorEnemyCharacter> SoftEnemyClassToSpawn, int32 NumToSpawn, const FVector& SpawnOrigin, float RandomSpawnRadius)
{
    //Task인스턴스 생성
    UAbilityTask_WaitSpawnEnemies* Node = NewAbilityTask<UAbilityTask_WaitSpawnEnemies>(OwningAbility);
    //입력받은 파라미터를 내부 변수에 저장
    Node->CachedEventTag = EventTag;
    Node->CachedSoftEnemyClassToSpawn = SoftEnemyClassToSpawn;
    Node->CachedNumToSpawn = NumToSpawn;
    Node->CachedSpawnOrigin = SpawnOrigin;
    Node->CachedRandomSpawnRadius = RandomSpawnRadius;
   
    return Node;
}

//태스크 실행 시
void UAbilityTask_WaitSpawnEnemies::Activate()
{
    //CachedEventTag에 해당되는 델리게이트 탐색, 생성
    FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);
    DelegateHandle = Delegate.AddUObject(this, &ThisClass::OnGameplayEventReceived);
}

//태스크 종료시
void UAbilityTask_WaitSpawnEnemies::OnDestroy(bool bInOwnerFinished)
{
    //델리데이트 탐색, 삭제
    FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);
    Delegate.Remove(DelegateHandle);
    Super::OnDestroy(bInOwnerFinished);
}

//CachedEventTag 이벤트 발생 시 호출되는 함수
void UAbilityTask_WaitSpawnEnemies::OnGameplayEventReceived(const FGameplayEventData* InPayload)
{
    //SoftPtr체크
    if (ensure(!CachedSoftEnemyClassToSpawn.IsNull()))
    {
        //비동기 로딩
        UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
            CachedSoftEnemyClassToSpawn.ToSoftObjectPath(),
            FStreamableDelegate::CreateUObject(this, &ThisClass::OnEnemyClassLoaded)
        );
    }
    else
    {
        //null이면 실패처리
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            DidNotSpawn.Broadcast(TArray<AWarriorEnemyCharacter*>());
        }
        EndTask();
    }
}

void UAbilityTask_WaitSpawnEnemies::OnEnemyClassLoaded()
{
    //로드된 클래스 가져오기
    UClass* LoadedClass = CachedSoftEnemyClassToSpawn.Get();
    //레벨 정보 가져오기
    UWorld* World = GetWorld();

    if (!LoadedClass || !World)
    {
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            //DidNotSpawn델리게이트 호출
            DidNotSpawn.Broadcast(TArray<AWarriorEnemyCharacter*>());
        }
        //종료
        EndTask();

        return;
    }

    //스폰된 적 배열
    TArray<AWarriorEnemyCharacter*> SpawnedEnemies;
    FActorSpawnParameters SpawnParam;
    //스폰위치에 다른 물체 있으면 가능한 범위에서 위치 조정
    SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;


    //소환
    for (int32 i = 0; i < CachedNumToSpawn; i++)
    {
        FVector RandomLocation;
        //Nav시스템을 사용해서 범위내에 도달 가능한 위치 찾기
        UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, CachedSpawnOrigin, RandomLocation, CachedRandomSpawnRadius);

        RandomLocation += FVector(0.f, 0.f, 150.f);

        //바라볼 방향 -> 소환한 보스의 전방
        const FRotator SpawnFacingRotation = AbilitySystemComponent->GetAvatarActor()->GetActorForwardVector().ToOrientationRotator();
        //액터 스폰
        AWarriorEnemyCharacter* SpawnedEnemy = World->SpawnActor<AWarriorEnemyCharacter>(LoadedClass, RandomLocation, SpawnFacingRotation, SpawnParam);

        //배열에 저장
        if (SpawnedEnemy)
        {
            SpawnedEnemies.Add(SpawnedEnemy);
        }
    }
    if (ShouldBroadcastAbilityTaskDelegates())
    {
        //적이 한명이라도 있으면
        if (!SpawnedEnemies.IsEmpty())
        {
            //배열전달
            OnSpawnFinished.Broadcast(SpawnedEnemies);
        }
        else
        {
            //존재하지 않으면 
            DidNotSpawn.Broadcast(TArray<AWarriorEnemyCharacter*>());
        }
    }
    //태스크 종료
    EndTask();
}
