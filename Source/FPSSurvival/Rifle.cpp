// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"
#include "ShooterCharacter.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"

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

	FHitResult Hit;
	if (GunTrace(Hit, Start, End))
	{
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 12, 8, FColor::Red, false, 5);
		// UE_LOG(LogTemp, Warning, TEXT("Hit component: %s"), *Hit.GetComponent()->GetName());
	}

	// add sounds
	// add SFX
}




