// Fill out your copyright notice in the Description page of Project Settings.


#include "GEExecCalc_DamageTaken.h"
#include "ActionRPG/AbilitySystem/WarriorAttributeSet.h"
#include "ActionRPG/Utils/ActionRPGGamePlayTags.h"
#include "ActionRPG/Utils/DebugHelper.h"

//ATTRIBUTE_CAPTURE -> GAS에서 액터가 가진 Attribute를 계산에 사용할 수 있도록 가져오는 기능
//Gameplay Effect가 적용되는 정확한 시점의 능력치 값을 가져오기 때문에 변한 어트리뷰트를 계산에 적용 가능

//Damage계산에 필요한 Attribute 구조체
struct FWarriorDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)

	FWarriorDamageCapture()
	{
		//                                                                                                           false로 설정하면 실시간 값, true면 효과가 생성된 시점의 값
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, AttackPower, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DefensePower, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DamageTaken, Target, false)
	}
};

//Singleton 접근자
static const FWarriorDamageCapture& GetWarriorDamageCapture()
{
	//static으로 선언 -> 캡쳐할 어트리뷰트의 정의를 담고 있어서 개임 내내 내용이 바뀔 필요가 없음
	static FWarriorDamageCapture WarriorDamageCapture;
	return WarriorDamageCapture;
}


UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().DamageTakenDef);
}

//ExecutionParams->가해자, 피해자 정보, 캡처된 어트리뷰트 등 계산에 필요한 데이터 포함
//FGameplayEffectCustomExecutionOutput->출력 파라미터
void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//Spec가져오기
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	// 어트리뷰트 값을 계산할 때 사용할 태그 정보
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetWarriorDamageCapture().AttackPowerDef, EvaluateParameters, SourceAttackPower);
	//Debug::Print(TEXT("SourceAttackPower"), SourceAttackPower);

	float BaseDamage = 0.f;
	int32 UsedLightAttckComboCount = 0;
	int32 UsedHeavyAttackComboCount = 0;

	// SetByCaller로 넘어온 값 확인
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		// 'BaseDamage' 태그가 있다면 그 값을 BaseDamage 변수에 저장
		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Public_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
			//Debug::Print(TEXT("BaseDamage"), BaseDamage);
		}

		// 'Light Attack' 태그가 있다면 그 값을 콤보 카운트 변수에 저장
		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			UsedLightAttckComboCount = TagMagnitude.Value;
			//Debug::Print(TEXT("UsedLightAttckComboCount"), UsedLightAttckComboCount);
		}

		// 'Heavy Attack' 태그가 있다면 그 값을 콤보 카운트 변수에 저장
		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Player_SetByCaller_AttackType_Heavy))
		{
			UsedHeavyAttackComboCount = TagMagnitude.Value;
			//Debug::Print(TEXT("UsedHeavyAttackComboCount"), UsedHeavyAttackComboCount);
		}
	}

	float TargetDefensePower = 0.f;
	// 이전에 정의한 DefensePower 캡처 설정을 사용최종 방어력 값 설정
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetWarriorDamageCapture().DefensePowerDef, EvaluateParameters, TargetDefensePower);
	//Debug::Print(TEXT("TargetDefensePower"), TargetDefensePower);

	//LightAttck 보너스 계산
	if (UsedLightAttckComboCount != 0)
	{
		//1타는 보너스가 없고 2타부터 5%씩 데미지 추가(0.05)		                                         (곱셈을 위해 값 보정)
		const float DamageIncreasePercentLight = (UsedLightAttckComboCount - 1) * 0.05 + 1.f;
		//데미지 계산
		BaseDamage *= DamageIncreasePercentLight;
		//Debug::Print(TEXT("ScaledBaseDamageLight"), BaseDamage);
	}

	if (UsedHeavyAttackComboCount != 0)
	{
		const float DamageIncreasePercentHeavy = UsedHeavyAttackComboCount * 0.15f + 1.f;
		BaseDamage *= DamageIncreasePercentHeavy;
		//Debug::Print(TEXT("ScaledBaseDamageHeavy"), BaseDamage);
	}

	//최종 데미지 산출
	const float FinalDamageDone = BaseDamage * SourceAttackPower / TargetDefensePower;
	//Debug::Print(TEXT("FinalDamageDone"), FinalDamageDone);

	if (FinalDamageDone > 0.f)
	{
		//DamageTaken 어트리뷰트 설정
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetWarriorDamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamageDone
			)
		);
	}
}
