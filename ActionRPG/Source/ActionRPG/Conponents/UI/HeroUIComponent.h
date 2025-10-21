// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRPG/Conponents/UI/PawnUIComponent.h"
#include "HeroUIComponent.generated.h"

//무기 아이콘 업데이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquippedWeaponChangedDelegate, TSoftObjectPtr<UTexture2D>, SoftWeaponIcon);


UCLASS()
class ACTIONRPG_API UHeroUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentRageChanged;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnEquippedWeaponChangedDelegate OnEquippedWeaponChanged;
};
