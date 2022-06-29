// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"
#include "ShooterCharacter.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

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

	// DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5);
	// UE_LOG(LogTemp, Warning, TEXT("%s Shooting!"), *GetActorNameOrLabel());

	TArray<FHitResult> HitArray;
	TArray<AActor*> HitArraySorted;
	int CurrentPenCount = 0;
	if (GunTrace(HitArray, Start, End))
	{
		int i = 0;
		while (i < HitArray.Num())
		{
			HitArraySorted.AddUnique(HitArray[i].GetActor());
			DrawDebugSphere(GetWorld(), HitArray[i].ImpactPoint, 16, 8, FColor::Red, false, 5);
			i++;
		}

		int index = 0;
		while (index < HitArraySorted.Num() && CurrentPenCount < TargetPenCount)
		{
			UGameplayStatics::ApplyDamage(HitArraySorted[index], Damage * FMath::Pow(0.9, index), UGameplayStatics::GetPlayerController(this, 0), GetOwner(), UDamageType::StaticClass());

			index++;
			CurrentPenCount++;
		}
		
		
		// UGameplayStatics::ApplyDamage(Hit.GetActor(), Damage, UGameplayStatics::GetPlayerController(this, 0), GetOwner(), UDamageType::StaticClass());
		// DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 12, 8, FColor::Red, false, 5);
		// UE_LOG(LogTemp, Warning, TEXT("Hit component: %s"), *Hit.GetComponent()->GetName());
	}

	// add sounds
	// add SFX
}




