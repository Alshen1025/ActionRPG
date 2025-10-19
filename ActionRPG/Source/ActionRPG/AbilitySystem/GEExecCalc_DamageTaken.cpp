// Fill out your copyright notice in the Description page of Project Settings.


#include "GEExecCalc_DamageTaken.h"
#include "ActionRPG/AbilitySystem/WarriorAttributeSet.h"
#include "ActionRPG/Utils/ActionRPGGamePlayTags.h"
#include "ActionRPG/Utils/DebugHelper.h"

//ATTRIBUTE_CAPTURE -> GAS���� ���Ͱ� ���� Attribute�� ��꿡 ����� �� �ֵ��� �������� ���
//Gameplay Effect�� ����Ǵ� ��Ȯ�� ������ �ɷ�ġ ���� �������� ������ ���� ��Ʈ����Ʈ�� ��꿡 ���� ����

//Damage��꿡 �ʿ��� Attribute ����ü
struct FWarriorDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)

	FWarriorDamageCapture()
	{
		//                                                                                                           false�� �����ϸ� �ǽð� ��, true�� ȿ���� ������ ������ ��
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, AttackPower, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DefensePower, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DamageTaken, Target, false)
	}
};

//Singleton ������
static const FWarriorDamageCapture& GetWarriorDamageCapture()
{
	//static���� ���� -> ĸ���� ��Ʈ����Ʈ�� ���Ǹ� ��� �־ ���� ���� ������ �ٲ� �ʿ䰡 ����
	static FWarriorDamageCapture WarriorDamageCapture;
	return WarriorDamageCapture;
}


UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().DamageTakenDef);
}

//ExecutionParams->������, ������ ����, ĸó�� ��Ʈ����Ʈ �� ��꿡 �ʿ��� ������ ����
//FGameplayEffectCustomExecutionOutput->��� �Ķ����
void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//Spec��������
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	// ��Ʈ����Ʈ ���� ����� �� ����� �±� ����
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetWarriorDamageCapture().AttackPowerDef, EvaluateParameters, SourceAttackPower);
	//Debug::Print(TEXT("SourceAttackPower"), SourceAttackPower);

	float BaseDamage = 0.f;
	int32 UsedLightAttckComboCount = 0;
	int32 UsedHeavyAttackComboCount = 0;

	// SetByCaller�� �Ѿ�� �� Ȯ��
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		// 'BaseDamage' �±װ� �ִٸ� �� ���� BaseDamage ������ ����
		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Public_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
			//Debug::Print(TEXT("BaseDamage"), BaseDamage);
		}

		// 'Light Attack' �±װ� �ִٸ� �� ���� �޺� ī��Ʈ ������ ����
		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			UsedLightAttckComboCount = TagMagnitude.Value;
			//Debug::Print(TEXT("UsedLightAttckComboCount"), UsedLightAttckComboCount);
		}

		// 'Heavy Attack' �±װ� �ִٸ� �� ���� �޺� ī��Ʈ ������ ����
		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Player_SetByCaller_AttackType_Heavy))
		{
			UsedHeavyAttackComboCount = TagMagnitude.Value;
			//Debug::Print(TEXT("UsedHeavyAttackComboCount"), UsedHeavyAttackComboCount);
		}
	}

	float TargetDefensePower = 0.f;
	// ������ ������ DefensePower ĸó ������ ������� ���� �� ����
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetWarriorDamageCapture().DefensePowerDef, EvaluateParameters, TargetDefensePower);
	//Debug::Print(TEXT("TargetDefensePower"), TargetDefensePower);

	//LightAttck ���ʽ� ���
	if (UsedLightAttckComboCount != 0)
	{
		//1Ÿ�� ���ʽ��� ���� 2Ÿ���� 5%�� ������ �߰�(0.05)		                                         (������ ���� �� ����)
		const float DamageIncreasePercentLight = (UsedLightAttckComboCount - 1) * 0.05 + 1.f;
		//������ ���
		BaseDamage *= DamageIncreasePercentLight;
		//Debug::Print(TEXT("ScaledBaseDamageLight"), BaseDamage);
	}

	if (UsedHeavyAttackComboCount != 0)
	{
		const float DamageIncreasePercentHeavy = UsedHeavyAttackComboCount * 0.15f + 1.f;
		BaseDamage *= DamageIncreasePercentHeavy;
		//Debug::Print(TEXT("ScaledBaseDamageHeavy"), BaseDamage);
	}

	//���� ������ ����
	const float FinalDamageDone = BaseDamage * SourceAttackPower / TargetDefensePower;
	//Debug::Print(TEXT("FinalDamageDone"), FinalDamageDone);

	if (FinalDamageDone > 0.f)
	{
		//DamageTaken ��Ʈ����Ʈ ����
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetWarriorDamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamageDone
			)
		);
	}
}
