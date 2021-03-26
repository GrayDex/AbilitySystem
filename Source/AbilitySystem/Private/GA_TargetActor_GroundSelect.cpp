// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_TargetActor_GroundSelect.h"
#include "Abilities/GameplayAbility.h"
#include "DrawDebugHelpers.h"
#include "Components/DecalComponent.h"


AGA_TargetActor_GroundSelect::AGA_TargetActor_GroundSelect()
{
	PrimaryActorTick.bCanEverTick = true; // Tick() is enable

	RootComp = CreateDefaultSubobject<USceneComponent>(FName("RootComp"));
	SetRootComponent(RootComp);
	Decal = CreateDefaultSubobject<UDecalComponent>(FName("Decal"));
	Decal->SetupAttachment(RootComp);
	AreaRadius = 200.f;
	Decal->DecalSize = FVector(AreaRadius);
}

void AGA_TargetActor_GroundSelect::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	FVector LookPoint;
	GetPlayerLookingPoint(LookPoint);
	Decal->SetWorldLocation(LookPoint);
	Decal->DecalSize = FVector(AreaRadius);
}

void AGA_TargetActor_GroundSelect::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
}

void AGA_TargetActor_GroundSelect::ConfirmTargetingAndContinue()
{
	FVector ViewLocation;
	GetPlayerLookingPoint(ViewLocation);

	TArray <FOverlapResult> Overlaps;
	TArray <TWeakObjectPtr<AActor>> OverlappingActors;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bTraceComplex = false;
	CollisionQueryParams.bReturnPhysicalMaterial = false;

	APawn* MasterPawn = MasterPC->GetPawn();
	if (MasterPawn) { CollisionQueryParams.AddIgnoredActor(MasterPawn->GetUniqueID()); }

	bool TryOverlap = GetWorld()->OverlapMultiByObjectType( 
		Overlaps, 
		ViewLocation, 
		FQuat::Identity, //FQuat::Identity is null rotator
		FCollisionObjectQueryParams(ECC_Pawn),
		FCollisionShape::MakeSphere(AreaRadius),
		CollisionQueryParams);

	if (TryOverlap)
	{
		for (int32 i = 0; i < Overlaps.Num(); ++i)
		{
			APawn* PawnOverlaped = Cast<APawn>(Overlaps[i].GetActor());
			if (PawnOverlaped && !OverlappingActors.Contains(PawnOverlaped))
			{
				OverlappingActors.Add(PawnOverlaped);
			}
		}
	}

	// create new struct.
	FGameplayAbilityTargetData_LocationInfo* CenterLocation = new FGameplayAbilityTargetData_LocationInfo();
	if (Decal) 
	{
		// init var
		CenterLocation->TargetLocation.LiteralTransform = Decal->GetComponentTransform();
		CenterLocation->TargetLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
	}

	if (OverlappingActors.Num() > 0)
	{
		FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlappingActors);

		// Add new created member in array. 
		// FGameplayAbilityTargetData_LocationInfo is child to FGameplayAbilityTargetData, 
		// so we can add it to Data
		TargetData.Add(CenterLocation); 

		TargetDataReadyDelegate.Broadcast(TargetData); // blueprint WaitTargetData subscribe this data for use in ability
	}
	else
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle(CenterLocation)); //only target location
	}
}

bool AGA_TargetActor_GroundSelect::GetPlayerLookingPoint(FVector& OutViewPoint)
{
	FVector ViewPoint;
	FRotator ViewRotation;
	MasterPC->GetPlayerViewPoint(ViewPoint, ViewRotation);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	APawn* MasterPawn = MasterPC->GetPawn();
	if (MasterPawn) { QueryParams.AddIgnoredActor(MasterPawn->GetUniqueID()); }

	bool TryTrace = GetWorld()->LineTraceSingleByChannel(HitResult, ViewPoint, ViewPoint + ViewRotation.Vector() * 10000.f, ECC_Visibility, QueryParams);

	OutViewPoint = FVector(0.f);
	if (TryTrace) { OutViewPoint = HitResult.ImpactPoint; }

	return TryTrace;
}
