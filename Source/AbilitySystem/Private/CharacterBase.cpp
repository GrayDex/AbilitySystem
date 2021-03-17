// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "AttributeSetBase.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");
	AttributeSetBaseComp = CreateDefaultSubobject<UAttributeSetBase>("AttributeSetBaseComp");
	bIsDead = false;
	TeamID = 255;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Initializing Character attributes in UI
	OnHealthChanged(AttributeSetBaseComp->Health.GetCurrentValue(), AttributeSetBaseComp->MaxHealth.GetCurrentValue());
	OnManaChanged(AttributeSetBaseComp->Mana.GetCurrentValue(), AttributeSetBaseComp->MaxMana.GetCurrentValue());
	OnStrenghtChanged(AttributeSetBaseComp->Strenght.GetCurrentValue(), AttributeSetBaseComp->MaxStrenght.GetCurrentValue());

	// Subscribe 
	AttributeSetBaseComp->OnHealthChange.AddDynamic(this, &ACharacterBase::OnHealthChanged);
	AttributeSetBaseComp->OnManaChange.AddDynamic(this, &ACharacterBase::OnManaChanged);
	AttributeSetBaseComp->OnStrenghtChange.AddDynamic(this, &ACharacterBase::OnStrenghtChanged);

	AddGameplayTag(FullHealthTag);
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) { Super::SetupPlayerInputComponent(PlayerInputComponent); }

void ACharacterBase::AcquireAbility(TSubclassOf<UGameplayAbility> AbilityToAcquire)
{
	if (!AbilityToAcquire) { UE_LOG(LogTemp, Error, TEXT("[Grayz] In %s AbilityToAquire is nullptr"), *FString(__FUNCTION__)); return; }
	if (AbilitySystemComp)
	{
		if (HasAuthority() && AbilityToAcquire) // HasAutorioty(), because this code not run in local mashine
		{
			FGameplayAbilitySpecDef SpecDef = FGameplayAbilitySpecDef();
			SpecDef.Ability = AbilityToAcquire;
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(SpecDef, 1);
			AbilitySystemComp->GiveAbility(AbilitySpec);
		}
		AbilitySystemComp->InitAbilityActorInfo(this, this);
	}
}

void ACharacterBase::OnHealthChanged(float Health, float MaxHealth)
{
	if (Health <= 0.0f && !bIsDead) 
	{
		bIsDead = true;
		Dead();
		BP_Die(); 
	}
	if (Health == MaxHealth)
	{
		AddGameplayTag(FullHealthTag);
	}
	else
	{
		RemoveGameplayTag(FullHealthTag);
	}

	BP_OnHealthChanged(Health, MaxHealth);
}

void ACharacterBase::OnManaChanged(float Mana, float MaxMana)
{
	BP_OnManaChanged(Mana, MaxMana);
}

void ACharacterBase::OnStrenghtChanged(float Strenght, float MaxStrenght)
{
	BP_OnStrenghtChanged(Strenght, MaxStrenght);
}

bool ACharacterBase::IsOtherHostile(ACharacterBase* Other)
{
	if (!Other) { UE_LOG(LogTemp, Warning, TEXT("[Grayz] In %s Other is nullptr"), *FString(__FUNCTION__)); return false; }
	return TeamID != Other->GetTeamID();
}

void ACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (NewController->IsPlayerController()) { TeamID = 0; }
}

void ACharacterBase::AddGameplayTag(FGameplayTag& TagToAdd)
{
	GetAbilitySystemComponent()->AddLooseGameplayTag(TagToAdd);
	GetAbilitySystemComponent()->SetTagMapCount(TagToAdd, 1); // only one instance this tag will be created
}

void ACharacterBase::RemoveGameplayTag(FGameplayTag& TagToRemove)
{
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(TagToRemove);
}

void ACharacterBase::Dead()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC) { PC->DisableInput(PC); }

	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC) { AIC->GetBrainComponent()->StopLogic("Dead"); }
}
