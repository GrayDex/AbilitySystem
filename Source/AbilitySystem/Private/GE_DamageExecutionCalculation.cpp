// Fill out your copyright notice in the Description page of Project Settings.


#include "GE_DamageExecutionCalculation.h"
#include "AttributeSetBase.h"

//// -------------------------------------------------------------------------
////	Helper macros for declaring attribute captures 
//// -------------------------------------------------------------------------

//#define DECLARE_ATTRIBUTE_CAPTUREDEF(P) \
//	FProperty* P##Property; \
//	FGameplayEffectAttributeCaptureDefinition P##Def; \

//#define DEFINE_ATTRIBUTE_CAPTUREDEF(S, P, T, B) \
//{ \
//	P##Property = FindFieldChecked<FProperty>(S::StaticClass(), GET_MEMBER_NAME_CHECKED(S, P)); \
//	P##Def = FGameplayEffectAttributeCaptureDefinition(P##Property, EGameplayEffectAttributeCaptureSource::T, B); \
//}

struct DamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Attack, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Armor, Target, true);
	}
};

static DamageStatics& GetDamageStatics()
{
	static DamageStatics DamageStaticVar;
	return DamageStaticVar;
}

UGE_DamageExecutionCalculation::UGE_DamageExecutionCalculation()
{
	HealthProperty = FindFieldChecked<FProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Health));
	HealthDef = FGameplayEffectAttributeCaptureDefinition(HealthProperty, EGameplayEffectAttributeCaptureSource::Target, true); // @InSnapshot = false: not updated

	RelevantAttributesToCapture.Add(HealthDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().AttackDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().ArmorDef);
}

void UGE_DamageExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	/*
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	Spec.CapturedSourceTags;
	FAggregatorEvaluateParameters Params;
	Params.SourceTags = Spec.CapturedSourceTags;
	*/

	float AttackMagnitude = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().AttackDef, FAggregatorEvaluateParameters(), AttackMagnitude); // Get Attack

	float ArmorMagnitude = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ArmorDef, FAggregatorEvaluateParameters(), ArmorMagnitude); // Get Armor

	// Section for calculating the mechanics of inflicting damage
	float FinalDamage = FMath::Clamp(AttackMagnitude - ArmorMagnitude, 0.0f, AttackMagnitude - ArmorMagnitude);
	UE_LOG(LogTemp, Warning, TEXT("[Grayz] In %s AttackMagnitude: %f"), *FString(__FUNCTION__), AttackMagnitude);
	
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(HealthProperty,EGameplayModOp::Additive, -FinalDamage)); // reduced target health by - 100
}
