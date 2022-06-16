// Fill out your copyright notice in the Description page of Project Settings.


#include "SpiderAIController.h"
#include "Kismet/GameplayStatics.h"


void ASpiderAIController::BeginPlay()
{
    Super::BeginPlay();

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    SetFocus(PlayerPawn);
    MoveToActor(PlayerPawn, 200);

    UE_LOG(LogTemp, Warning, TEXT("Player pawn: %s"), *PlayerPawn->GetActorNameOrLabel());

}
