// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilityBase.h"

FGameplayAbilityInfo UGameplayAbilityBase::GetAbilityInfo()
{
	UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect();
	UGameplayEffect* CostEffect = GetCostGameplayEffect();
	if (CooldownEffect && CostEffect)
	{
		float CooldownDuration = 0;
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(1, CooldownDuration); // get duration

		float Cost = 0;
		if (CostEffect->Modifiers.Num() > 0)
		{
			CostEffect->Modifiers[0].ModifierMagnitude.GetStaticMagnitudeIfPossible(1, Cost); // get cost
			FString AttributeName = CostEffect->Modifiers[0].Attribute.AttributeName; // get cost type name
			return FGameplayAbilityInfo(CooldownDuration, Cost, GetCostTypeByName(AttributeName), UIMaterial, GetClass());
		}
	}
	return FGameplayAbilityInfo();
}

EAbilityCostType UGameplayAbilityBase::GetCostTypeByName(FString Name)
{
	EAbilityCostType CostType = EAbilityCostType::Health;
	// get cost type by name
	if (Name == "Health")
	{
		CostType = EAbilityCostType::Health;
	}
	else if (Name == "Mana")
	{
		CostType = EAbilityCostType::Mana;
	}
	else if (Name == "Strenght")
	{
		CostType = EAbilityCostType::Strenght;
	}
	return CostType;
}
