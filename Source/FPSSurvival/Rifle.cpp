// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"
#include "ShooterCharacter.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "BaseEnemy.h"
#include "Particles/ParticleSystemComponent.h"

ARifle::ARifle()
{
    MuzzleFlashParticleComp = CreateDefaultSubobject<UParticleSystemComponent>("Muzzle Flash Particle");
	MuzzleFlashParticleComp->SetupAttachment(GetMuzzle());
	MuzzleFlashParticleComp->SetVisibility(false);
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

	bool bHitEnemy = false;

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
			
			ABaseEnemy* Enemy = Cast<ABaseEnemy>(HitArraySorted[index].GetActor());
			if (bHitEnemy == false)
			{
				if (Enemy != nullptr)
				{
					bHitEnemy = true;
				}
			}

			if (FleshImpactSound && Enemy != nullptr)
			{
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FleshImpactSound, HitArraySorted[index].ImpactPoint);
			}
			else if (ConcreteImpactSound)
			{
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ConcreteImpactSound, HitArraySorted[index].ImpactPoint);
			}
			index++;
			CurrentPenCount++;
		}

		if (bHitEnemy)
		{
			ShowHitmarker();
			FTimerHandle HitmarkerHandle;
			GetWorldTimerManager().SetTimer(HitmarkerHandle, this, &ABaseGun::HideHitmarker, 0.15, false);
		}

		ShowMuzzleFlash();
	}

	if (ShotSound)
	{
		UGameplayStatics::SpawnSoundAttached(ShotSound, GetMesh(), TEXT("MuzzleFlashSocket"));
	}
	
}

void ARifle::ShowMuzzleFlash()
{
	MuzzleFlashParticleComp->SetVisibility(true);
}

void ARifle::HideMuzzleFlash()
{
	MuzzleFlashParticleComp->SetVisibility(false);
}




