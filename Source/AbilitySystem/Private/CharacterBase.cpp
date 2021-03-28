// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "AttributeSetBase.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.h"
#include "AbilityTypes.h"
#include "GameplayAbilityBase.h"

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


void ACharacterBase::Apply_GESH_To_TDH(const FGameplayEffectSpecHandle& GameplayEffect, const FGameplayAbilityTargetDataHandle& TargetData)
{
	for (TSharedPtr<FGameplayAbilityTargetData>Data : TargetData.Data)
	{
		Data->ApplyGameplayEffectSpec(*GameplayEffect.Data.Get());
	}
}


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

void ACharacterBase::AcquareAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilityToAquire)
{
	if (AbilityToAquire.Num() < 1) { UE_LOG(LogTemp, Error, TEXT("[Grayz] In %s AbilityToAquire is empty"), *FString(__FUNCTION__)); return; }
	for (TSubclassOf<UGameplayAbility> AbilityItem : AbilityToAquire)
	{
		AcquireAbility(AbilityItem);
		if (AbilityItem->IsChildOf(UGameplayAbilityBase::StaticClass())) // if class ability is UGameplayAbilityBase
		{
			TSubclassOf<UGameplayAbilityBase> AbilityBaseClass = *AbilityItem;
			if (AbilityBaseClass != nullptr)
			{
				AddAbilityToUI(AbilityBaseClass);
			}
		}
	}
}

void ACharacterBase::AddAbilityToUI(TSubclassOf<UGameplayAbilityBase> AbilityToAdd)
{
	APlayerControllerBase* PlayerControllerBase = Cast<APlayerControllerBase>(GetController());
	if (PlayerControllerBase)
	{
		UGameplayAbilityBase* AbilityInstance = AbilityToAdd.Get()->GetDefaultObject<UGameplayAbilityBase>();
		if (AbilityInstance)
		{
			FGameplayAbilityInfo AbilityInfo = AbilityInstance->GetAbilityInfo();
			UE_LOG(LogTemp, Warning, TEXT("[Grayz] In %s Cost: %f"), *FString(__FUNCTION__), AbilityInfo.Cost);
			UE_LOG(LogTemp, Warning, TEXT("[Grayz] In %s CooldownDuration: %f"), *FString(__FUNCTION__), AbilityInfo.CooldownDuration);
			PlayerControllerBase->AddAbilityToUI(AbilityInfo);
		}
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
	DisableInputControl();
}

void ACharacterBase::DisableInputControl()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC) 
	{
		PC->DisableInput(PC); 
	}
	else
	{
		AAIController* AIC = Cast<AAIController>(GetController());
		if (AIC) { AIC->GetBrainComponent()->StopLogic("Dead"); }
	}
}

void ACharacterBase::EnableInputControl()
{
	if (!bIsDead)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			PC->EnableInput(PC);
		}
		else
		{
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC) { AIC->GetBrainComponent()->RestartLogic(); }
		}
	}
}

void ACharacterBase::HitStun(float StunDuration)
{
	DisableInputControl();
	GetWorldTimerManager().SetTimer(StunTimeHandle, this, &ACharacterBase::EnableInputControl, StunDuration, false);
}
