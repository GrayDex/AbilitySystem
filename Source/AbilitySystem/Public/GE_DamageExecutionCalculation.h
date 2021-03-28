// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GE_DamageExecutionCalculation.generated.h"


UCLASS()
class ABILITYSYSTEM_API UGE_DamageExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	UGE_DamageExecutionCalculation();

public:
	FProperty* HealthProperty;
	FGameplayEffectAttributeCaptureDefinition HealthDef;

	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
};
