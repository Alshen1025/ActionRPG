// Fill out your copyright notice in the Description page of Project Settings.


#include "InputConfigDataAsset.h"


UInputAction* UInputConfigDataAsset::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	//입력 구조체 배열 순회
	for (const FWarriorInputActionConfig& InputActionConfig : NativeInputActions)
	{
		//태그로 해당 InputAction가져오기
		if (InputActionConfig.InputTag == InInputTag && InputActionConfig.InputAction)
		{
			return InputActionConfig.InputAction;
		}
	}

	return nullptr;
}