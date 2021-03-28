// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GATActorAroundOwner.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEM_API AGATActorAroundOwner : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "GATActorAroundOwner")
	float AreaRadius;

	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;


};
