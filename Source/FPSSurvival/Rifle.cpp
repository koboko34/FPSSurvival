// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"
#include "ShooterCharacter.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ARifle::ARifle()
{
    
}

void ARifle::BeginPlay()
{
    Super::BeginPlay();
}

void ARifle::PullTrigger(AShooterCharacter* Player)
{
    if (GetOwner() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Gun %s has no owner"), *GetActorNameOrLabel());
		return;
	}
		
	if (Player == nullptr)
	{
		Player = Cast<AShooterCharacter>(GetOwner());
	}

	if (Player == nullptr)
	{
		return;
	}

    FVector Start = Player->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector End = Start + (Player->GetFirstPersonCameraComponent()->GetForwardVector() * Range);

	TArray<FHitResult> HitArray;
	TArray<FHitResult> HitArraySorted;
	int CurrentPenCount = 0;
	if (GunTrace(HitArray, Start, End))
	{
		int i = 0;
		while (i < HitArray.Num())
		{
			HitArraySorted.Add(HitArray[i]);
			// DrawDebugSphere(GetWorld(), HitArray[i].ImpactPoint, 16, 8, FColor::Red, false, 5);
			i++;
		}

		
		FVector ShotDirection = -UKismetMathLibrary::FindLookAtRotation(Start, End).Vector();

		int index = 0;
		while (index < HitArraySorted.Num() && CurrentPenCount < TargetPenCount)
		{
			UGameplayStatics::ApplyDamage(HitArraySorted[index].GetActor(), Damage * FMath::Pow(0.9, index), UGameplayStatics::GetPlayerController(this, 0), GetOwner(), UDamageType::StaticClass());
			if (ImpactParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, HitArraySorted[index].ImpactPoint, ShotDirection.Rotation());
			}
			index++;
			CurrentPenCount++;
		}
	}

	// add sounds

}




