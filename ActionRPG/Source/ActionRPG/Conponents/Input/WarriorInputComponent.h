// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "ActionRPG/DataAsset/InputConfigDataAsset.h"
#include "WarriorInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UWarriorInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(const UInputConfigDataAsset* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func);

	template<class UserObject, typename PressedFunc, typename ReleasedFunc>
	void BindAbilityInputAction(const UInputConfigDataAsset* InInputConfig, UserObject* ContextObject, PressedFunc InputPressedFunc, ReleasedFunc InputReleasedFunc);

};
//Inline에 선언
		//               클래스                 클래스의 멤버 함수
template<class UserObject, typename CallbackFunc>
inline void UWarriorInputComponent::BindNativeInputAction(const UInputConfigDataAsset* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
{
	//InputConfig확인
	checkf(InInputConfig, TEXT("Input config data asset is null,can not proceed with binding"));

	//Tag를 이용해 바인딩 할 InputAction 찾기
	if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
	{
		//바인딩 실행
		BindAction(FoundAction, TriggerEvent, ContextObject, Func);
	}
}

template<class UserObject, typename PressedFunc, typename ReleasedFunc>
inline void UWarriorInputComponent::BindAbilityInputAction(const UInputConfigDataAsset* InInputConfig, UserObject* ContextObject, PressedFunc InputPressedFunc, ReleasedFunc InputReleasedFunc)
{
	checkf(InInputConfig, TEXT("Input config data asset is null,can not proceed with binding"));

	for (const FWarriorInputActionConfig& AbilityInputActionConfig : InInputConfig->AbilityInputActions)
	{
		if (!AbilityInputActionConfig.IsVaild()) continue;
		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Started, ContextObject, InputPressedFunc, AbilityInputActionConfig.InputTag);
		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Completed, ContextObject, InputReleasedFunc, AbilityInputActionConfig.InputTag);
	}
}
