// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemInterface.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemComponent.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
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

	// This component need to activate and execution abilities
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grayz")
	UAbilitySystemComponent* AbilitySystemComp;

	// Contains attributes character: health, mana, etc.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grayz")
	UAttributeSetBase* AttributeSetBaseComp;

	/** Returns the ability system component to use for this actor. It may live on another actor, such as a Pawn using the PlayerState's component */
	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComp; };
	
	// Create ability to character
	UFUNCTION(BlueprintCallable, Category = "Grayz")
	void AcquireAbility(TSubclassOf<UGameplayAbility> AbilityToAquire);

	// Binded to broadcast OnHealthChanged
	UFUNCTION()
		void OnHealthChanged(float Health, float MaxHealth);
	// Called in void OnHealthChanged() when OnHealthChange broadcasted
	UFUNCTION(BlueprintImplementableEvent, Category = "Grayz", meta = (DisplayName = "OnHealthChanged"))
		void BP_OnHealthChanged(float Health, float MaxHealth);

	// Binded to broadcast OnManaChanged
	UFUNCTION()
		void OnManaChanged(float Mana, float MaxMana);
	// Called in void OnManaChanged() when OnManaChange broadcasted
	UFUNCTION(BlueprintImplementableEvent, Category = "Grayz", meta = (DisplayName = "OnManaChanged"))
		void BP_OnManaChanged(float Mana, float MaxMana);

	// Binded to broadcast OnStrenghtChanged
	UFUNCTION()
		void OnStrenghtChanged(float Strenght, float MaxStrenght);
	// Called in void OnStrenghtChanged() when OnStrenghtChange broadcasted
	UFUNCTION(BlueprintImplementableEvent, Category = "Grayz", meta = (DisplayName = "OnStrenghtChanged"))
		void BP_OnStrenghtChanged(float Strenght, float MaxStrenght);

	UFUNCTION(BlueprintImplementableEvent, Category = "Grayz", meta = (DisplayName = "Die"))
	void BP_Die();

	// Other actor is enemy?
	UFUNCTION(BlueprintCallable, Category = "Grayz")
	bool IsOtherHostile(ACharacterBase* Other);

	// Param to detect enemy or ally
	FORCEINLINE uint8 GetTeamID() const { return TeamID; };

	void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable, Category = "Grayz")
		void AddGameplayTag(FGameplayTag& TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "Grayz")
		void RemoveGameplayTag(FGameplayTag& TagToRemove);

	// This will be blocked ability for use
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grayz")
		FGameplayTag FullHealthTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grayz")
		bool bIsDead;


protected:
	uint8 TeamID;
	void Dead();
};
