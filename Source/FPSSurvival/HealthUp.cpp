// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthUp.h"
#include "Components/CapsuleComponent.h"
#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"

void AHealthUp::BeginPlay()
{
    Super::BeginPlay();

    GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AHealthUp::OnBeginOverlap);
}

void AHealthUp::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor == PlayerCharacter)
    {
        // logic here

        if (PlayerCharacter->OnHealthUp(HealthToRestore))
        {
            if (SoundEffect)
            {
                UGameplayStatics::PlaySound2D(GetWorld(), SoundEffect);
            }
            Destroy();
        }
    }
}