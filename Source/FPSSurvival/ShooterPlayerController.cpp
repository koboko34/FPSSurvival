// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AShooterPlayerController::AShooterPlayerController()
{
    // PrimaryActorTick.bCanEverTick = false;
    // PrimaryActorTick.bStartWithTickEnabled = false;
}

void AShooterPlayerController::BeginPlay()
{
    if (AbilityUltClass)
    {
        AbilityUlt = CreateWidget(this, AbilityUltClass);
    }
    
    if (HUDClass)
    {
        HUD = CreateWidget(this, HUDClass);
        HUD->AddToViewport();
    }

    if (PauseMenuClass)
    {
        PauseMenu = CreateWidget(this, PauseMenuClass);
    }

    BloodEffectDelegate = FTimerDelegate::CreateUObject(this, &AShooterPlayerController::SetBloodEffectNull);

}

void AShooterPlayerController::ShowInfo(FString MessageToShow)
{
    OnInfoDelegate.Broadcast(MessageToShow);
}

void AShooterPlayerController::ShowPauseMenu()
{
    PauseMenu->AddToViewport();
}

void AShooterPlayerController::HidePauseMenu()
{
    PauseMenu->RemoveFromParent();
}

void AShooterPlayerController::ShowAbilityUlt()
{
    AbilityUlt->AddToViewport(-3);
}

void AShooterPlayerController::HideAbilityUlt()
{
    AbilityUlt->RemoveFromParent();
}

void AShooterPlayerController::ShowBloodEffect()
{
    if (BloodEffectClass == nullptr)
    {
        return;
    }

    FTimerHandle BloodEffectHandle;

    if (BloodEffect != nullptr)
    {
        GetWorld()->GetTimerManager().ClearTimer(BloodEffectHandle);
        SetBloodEffectNull();
    }

    BloodEffect = CreateWidget(this, BloodEffectClass);
    BloodEffect->AddToViewport(-1);

    GetWorld()->GetTimerManager().SetTimer(BloodEffectHandle, BloodEffectDelegate, 1, false);
}

void AShooterPlayerController::ShowDeathEffect()
{
    if (DeathEffectClass == nullptr)
    {
        return;
    }

    DeathEffect = CreateWidget(this, DeathEffectClass);
    DeathEffect->AddToViewport(-1);
}

void AShooterPlayerController::SetBloodEffectNull()
{
    if (BloodEffect == nullptr)
    {
        return;
    }
    
    BloodEffect->RemoveFromParent();
    BloodEffect = nullptr;
}

void AShooterPlayerController::ShowHitmarker()
{
	bMakeHitmarkerVisible = true;
    GetWorld()->GetTimerManager().ClearTimer(HitmarkerHandle);
    GetWorld()->GetTimerManager().SetTimer(HitmarkerHandle, this, &AShooterPlayerController::HideHitmarker, 0.15, false);

    if (HitmarkerSound == nullptr)
    {
        return;
    }

    UGameplayStatics::PlaySound2D(this, HitmarkerSound);
}

void AShooterPlayerController::HideHitmarker()
{
	bMakeHitmarkerVisible = false;
}

void AShooterPlayerController::ShowKillmarker()
{
	bMakeKillmarkerVisible = true;
    GetWorld()->GetTimerManager().ClearTimer(KillmarkerHandle);
    GetWorld()->GetTimerManager().SetTimer(KillmarkerHandle, this, &AShooterPlayerController::HideKillmarker, 0.15, false);

    if (HitmarkerSound == nullptr)
    {
        return;
    }

    UGameplayStatics::PlaySound2D(this, HitmarkerSound);
}

void AShooterPlayerController::HideKillmarker()
{
	bMakeKillmarkerVisible = false;
}
