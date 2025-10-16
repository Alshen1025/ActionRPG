// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ActionRPG/AbilitySystem/WarriorAbilitySystemComponent.h"

UWarriorAbilitySystemComponent* UWarriorFunctionLibrary::NativeGetWarriorASCFromActior(AActor* Actor)
{
	check(Actor);

	//                                                                                           UAbilitySystemBlueprintLibrary�⺻ �Լ�
	return CastChecked<UWarriorAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor));
}

//���Ͱ� �±׸� ������ ���� ���� ��쿡�� �±� �߰�
void UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActior(InActor);

	//TagToAdd�� ��ġ�ϴ� �±� ���� ���� Ȯ��
	if (!ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

//���Ͱ� �±׸� ������ �ִ� ��쿡 �±� ����
void UWarriorFunctionLibrary::RemoveGameplayFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActior(InActor);

	if (ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

//���Ͱ� Ư�� �±׸� ������ �ִ��� Ȯ�� : C++
bool UWarriorFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActior(InActor);

	return ASC->HasMatchingGameplayTag(TagToCheck);
}

//���Ͱ� Ư�� �±׸� ������ �ִ��� Ȯ�� : BluePrint
void UWarriorFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EWarriorConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EWarriorConfirmType::Yes : EWarriorConfirmType::No;
}
