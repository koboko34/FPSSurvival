// Fill out your copyright notice in the Description page of Project Settings.


#include "Launcher.h"
#include "Projectile.h"
#include "ShooterCharacter.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"



ALauncher::ALauncher()
{
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;
	
	MuzzleFlashParticle = CreateDefaultSubobject<UParticleSystemComponent>("Muzzle Flash Particle");
	MuzzleFlashParticle->SetupAttachment(GetMesh(), TEXT("MuzzleFlashSocket"));
	MuzzleFlashParticle->SetVisibility(false);
}

void ALauncher::BeginPlay()
{
    Super::BeginPlay();

	MuzzleDelegate = FTimerDelegate::CreateUObject(this, &ALauncher::HideMuzzleFlash);
}

void ALauncher::PullTrigger(AShooterCharacter* Player)
{
    FVector Start = Player->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector End = Start + (Player->GetFirstPersonCameraComponent()->GetForwardVector() * Range);

    FVector Target = End;

	// DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5);
	// UE_LOG(LogTemp, Warning, TEXT("%s Shooting!"), *GetActorNameOrLabel());

	FHitResult Hit;
    FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(PlayerCharacter);
	Params.AddIgnoredComponent(PlayerCharacter->GetCapsuleComponent());
	Params.AddIgnoredComponent(GetMesh());
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
	if (Hit.IsValidBlockingHit())
	{
		// DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 12, 8, FColor::Red, false, 5);
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

    Projectile->SetOwner(this);
	
	if (LauncherShotSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), LauncherShotSound);
	}
    
	ShowMuzzleFlash();
	FTimerHandle MuzzleHandle;
	GetWorldTimerManager().SetTimer(MuzzleHandle, MuzzleDelegate, 0.1, false);
}

void ALauncher::ShowMuzzleFlash()
{
	MuzzleFlashParticle->SetVisibility(true);
}

void ALauncher::HideMuzzleFlash()
{
	MuzzleFlashParticle->SetVisibility(false);
}
