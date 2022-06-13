// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "Blueprint/UserWidget.h"


void AShooterPlayerController::BeginPlay()
{
    if (HUDClass)
    {
        HUD = CreateWidget(this, HUDClass);
        HUD->AddToViewport();
    }
}
