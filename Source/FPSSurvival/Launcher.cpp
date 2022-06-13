// Fill out your copyright notice in the Description page of Project Settings.


#include "Launcher.h"
#include "Projectile.h"
#include "ShooterCharacter.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"


ALauncher::ALauncher()
{
    
}

void ALauncher::BeginPlay()
{
    Super::BeginPlay();
}

void ALauncher::PullTrigger(AShooterCharacter* Player)
{
    FVector Start = Player->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector End = Start + (Player->GetFirstPersonCameraComponent()->GetForwardVector() * Range);

    FVector Target = End;

	// DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5);
	// UE_LOG(LogTemp, Warning, TEXT("%s Shooting!"), *GetActorNameOrLabel());

	FHitResult Hit;
	if (GunTrace(Hit, Start, End))
	{
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 12, 8, FColor::Red, false, 5);
        Target = Hit.ImpactPoint;
		// UE_LOG(LogTemp, Warning, TEXT("Hit component: %s"), *Hit.GetComponent()->GetName());
	}



    FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetMuzzle()->GetComponentLocation(), Target);

    AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>
    (
        ProjectileClass,
        GetMuzzle()->GetComponentLocation(),
        LookAtRotation
    );
	
    // add sounds
	// add SFX
}
