// Fill out your copyright notice in the Description page of Project Settings.


#include "GATActorAroundOwner.h"
#include "Abilities/GameplayAbility.h"

void AGATActorAroundOwner::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
}

void AGATActorAroundOwner::ConfirmTargetingAndContinue()
{
	APawn* OwningPawn = MasterPC->GetPawn();
	if (!OwningPawn) 
	{ 
		UE_LOG(LogTemp, Warning, TEXT("[Grayz] In %s OwningPawn is nullptr"), *FString(__FUNCTION__)); 
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
		return;
	}

	FVector OwnerLocation = OwningPawn->GetActorLocation();
	TArray <FOverlapResult> Overlaps;
	TArray <TWeakObjectPtr<AActor>> OverlappingActors;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bTraceComplex = false;
	CollisionQueryParams.bReturnPhysicalMaterial = false;

	APawn* MasterPawn = MasterPC->GetPawn();
	if (MasterPawn) { CollisionQueryParams.AddIgnoredActor(MasterPawn->GetUniqueID()); }

	bool TryOverlap = GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		OwnerLocation,
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
	if (OverlappingActors.Num() > 0)
	{
		FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlappingActors);
		TargetDataReadyDelegate.Broadcast(TargetData); // blueprint WaitTargetData subscribe this data for use in ability
	}
	else
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
	}

}
