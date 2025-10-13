// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
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

		//Inline에 선언
		//               클래스                 클래스의 멤버 함수
		template<class UserObject, typename CallbackFunc>
		inline void BindNativeInputAction(const UInputConfigDataAsset* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
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
	
};
