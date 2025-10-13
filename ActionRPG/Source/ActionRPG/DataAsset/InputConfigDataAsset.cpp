// Fill out your copyright notice in the Description page of Project Settings.


#include "InputConfigDataAsset.h"


UInputAction* UInputConfigDataAsset::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	//�Է� ����ü �迭 ��ȸ
	for (const FWarriorInputActionConfig& InputActionConfig : NativeInputActions)
	{
		//�±׷� �ش� InputAction��������
		if (InputActionConfig.InputTag == InInputTag && InputActionConfig.InputAction)
		{
			return InputActionConfig.InputAction;
		}
	}

	return nullptr;
}