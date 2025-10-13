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

		//Inline�� ����
		//               Ŭ����                 Ŭ������ ��� �Լ�
		template<class UserObject, typename CallbackFunc>
		inline void BindNativeInputAction(const UInputConfigDataAsset* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
		{
			//InputConfigȮ��
			checkf(InInputConfig, TEXT("Input config data asset is null,can not proceed with binding"));

			//Tag�� �̿��� ���ε� �� InputAction ã��
			if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
			{
				//���ε� ����
				BindAction(FoundAction, TriggerEvent, ContextObject, Func);
			}
		}
	
};
