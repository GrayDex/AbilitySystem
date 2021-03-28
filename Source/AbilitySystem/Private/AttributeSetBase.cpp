// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"

UAttributeSetBase::UAttributeSetBase()
	:Health(200.f),
	MaxHealth(200.f),
	Mana(100.f),
	MaxMana(150.f),
	Strenght(200.f),
	MaxStrenght(250.f),
	Attack(20.f),
	Armor(5.f)
{

}

void UAttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) ///called after execute gameplay effect, that modified any attibute in this AttributeSetBase instance
{
	// Health attribute broadcast
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Health)))// if modified is Health
	{
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.f, MaxHealth.GetCurrentValue())); // clamp value, for not set minus value
		Health.SetBaseValue(FMath::Clamp(Health.GetBaseValue(), 0.f, MaxHealth.GetCurrentValue())); 
		OnHealthChange.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());
	}

	// Mana attribute broadcast
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Mana)))// if modified is Health
	{
		Mana.SetCurrentValue(FMath::Clamp(Mana.GetCurrentValue(), 0.f, MaxMana.GetCurrentValue())); // clamp value, for not set minus value
		Mana.SetBaseValue(FMath::Clamp(Mana.GetBaseValue(), 0.f, MaxMana.GetCurrentValue()));
		OnManaChange.Broadcast(Mana.GetCurrentValue(), MaxMana.GetCurrentValue());
	}

	// Strenght attribute broadcast
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Strenght)))// if modified is Health
	{
		Strenght.SetCurrentValue(FMath::Clamp(Strenght.GetCurrentValue(), 0.f, MaxStrenght.GetCurrentValue())); // clamp value, for not set minus value
		Strenght.SetBaseValue(FMath::Clamp(Strenght.GetBaseValue(), 0.f, MaxStrenght.GetCurrentValue()));
		OnStrenghtChange.Broadcast(Strenght.GetCurrentValue(), MaxStrenght.GetCurrentValue());
	}
}
