// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GA_TargetActor_GroundSelect.generated.h"

/**
 * Make TargetDataHandleFromActors, that are within the radius of the given variable Radius.
 * Class used in AreaOfEffect skills.
 */
UCLASS()
class ABILITYSYSTEM_API AGA_TargetActor_GroundSelect : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	AGA_TargetActor_GroundSelect();

	// Radius for AOE ability
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "Grayz")
	float AreaRadius;

	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grayz")
	UDecalComponent* Decal;

	virtual void Tick(float DeltaSeconds) override;
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;

	UFUNCTION(BlueprintCallable, Category = "Grayz")
	bool GetPlayerLookingPoint(FVector& OutViewPoint);


};
