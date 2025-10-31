// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyUIComponent.h"
#include "ActionRPG/Widget/WarriorWidgetBase.h"

void UEnemyUIComponent::RegisterEnemyDrawnWidget(UWarriorWidgetBase* InWidgetToRegister)
{
	EnemyDrawnWidgets.Add(InWidgetToRegister);
}

void UEnemyUIComponent::RemoveEnemyDrawnWidgetsIfAny()
{
	if (EnemyDrawnWidgets.IsEmpty())
	{
		return;
	}

	for (UWarriorWidgetBase* DrawnWidget : EnemyDrawnWidgets)
	{
		if (DrawnWidget)
		{
			DrawnWidget->RemoveFromParent();
		}
	}
	EnemyDrawnWidgets.Empty();
}
