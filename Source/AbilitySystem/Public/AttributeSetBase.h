// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AttributeSetBase.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangeDelegate, float, Health, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnManaChangeDelegate, float, Mana, float, MaxMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStrenghtChangeDelegate, float, Strenght, float, MaxStrenght);

UCLASS()
class ABILITYSYSTEM_API UAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

	UAttributeSetBase();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
		FGameplayAttributeData Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
		FGameplayAttributeData MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
		FGameplayAttributeData Mana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
		FGameplayAttributeData MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
		FGameplayAttributeData Strenght;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
		FGameplayAttributeData MaxStrenght;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
		FGameplayAttributeData Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
		FGameplayAttributeData Armor;


	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	FOnHealthChangeDelegate OnHealthChange;
	FOnManaChangeDelegate OnManaChange;
	FOnStrenghtChangeDelegate OnStrenghtChange;
};
