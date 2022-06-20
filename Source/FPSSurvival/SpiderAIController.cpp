// Fill out your copyright notice in the Description page of Project Settings.


#include "SpiderAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Spider.h"


void ASpiderAIController::BeginPlay()
{
    Super::BeginPlay();

    if (AIBehavior == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("early return"));
        return;
    }

    RunBehaviorTree(AIBehavior);
    GetBlackboardComponent()->SetValueAsBool(TEXT("TrueBool"), true);
    GetBlackboardComponent()->SetValueAsBool(TEXT("FalseBool"), false);
    GetBlackboardComponent()->SetValueAsBool(TEXT("InRange"), false);

}

void ASpiderAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
}
