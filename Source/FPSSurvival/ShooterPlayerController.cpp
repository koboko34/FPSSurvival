// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "Blueprint/UserWidget.h"


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
    AbilityUlt->AddToViewport();
}

void AShooterPlayerController::HideAbilityUlt()
{
    AbilityUlt->RemoveFromParent();
}
