// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemInterface.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemComponent.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/Abilities/GameplayAbility.h"
#include "CharacterBase.generated.h"

class UAttributeSetBase;

UCLASS()
class ABILITYSYSTEM_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Comment.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grayz")
	UAbilitySystemComponent* AbilitySystemComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grayz")
	UAttributeSetBase* AttributeSetBaseComp;

	/** Returns the ability system component to use for this actor. It may live on another actor, such as a Pawn using the PlayerState's component */
	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComp; };
	
	// Comment.
	UFUNCTION(BlueprintCallable, Category = "Grayz")
	void AcquireAbility(TSubclassOf<UGameplayAbility> AbilityToAquire);

	UFUNCTION()
	void OnHealthChanged(float Health, float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "Grayz", meta = (DisplayName = "OnHealthChanged"))
	void BP_OnHealthChanged(float Health, float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "Grayz", meta = (DisplayName = "Die"))
	void BP_Die();

	UFUNCTION(BlueprintCallable, Category = "Grayz")
	bool IsOtherHostile(ACharacterBase* Other);

	FORCEINLINE uint8 GetTeamID() const { return TeamID; };

	void PossessedBy(AController* NewController) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grayz")
	bool bIsDead;
	uint8 TeamID;
	void Dead();
};
