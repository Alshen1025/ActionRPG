// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorController.h"

AWarriorController::AWarriorController()
{
	//TeamID����
	HeroTeamID = FGenericTeamId(0);
}


FGenericTeamId AWarriorController::GetGenericTeamId() const
{
	return HeroTeamID;
}