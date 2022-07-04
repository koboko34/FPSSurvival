// Fill out your copyright notice in the Description page of Project Settings.


#include "MaxAmmo.h"
#include "Components/CapsuleComponent.h"
#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"

void AMaxAmmo::BeginPlay()
{
    Super::BeginPlay();

    GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMaxAmmo::OnBeginOverlap);
}

void AMaxAmmo::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor == PlayerCharacter)
    {
        // logic here
        PlayerCharacter->OnMaxAmmo();

        if (SoundEffect)
        {
            UGameplayStatics::PlaySound2D(GetWorld(), SoundEffect);
        }

        Destroy();
    }
}
