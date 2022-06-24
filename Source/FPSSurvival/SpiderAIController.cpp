// Fill out your copyright notice in the Description page of Project Settings.


#include "SpiderAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Spider.h"
#include "BrainComponent.h"


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
    GetBlackboardComponent()->SetValueAsBool(TEXT("IsAlive"), true);
    GetBlackboardComponent()->SetValueAsBool(TEXT("IsStunned"), false);

}

void ASpiderAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    ASpider* SpiderOwner = Cast<ASpider>(GetPawn());
    if (SpiderOwner == nullptr)
    {
        return;
    }

    if (SpiderOwner->IsStunned() == false)
    {
        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
    }

    GetBlackboardComponent()->SetValueAsBool(TEXT("IsAlive"), SpiderOwner->IsAlive());
    GetBlackboardComponent()->SetValueAsBool(TEXT("IsStunned"), SpiderOwner->IsStunned());

    if (SpiderOwner->IsAlive() == false)
    {
        GetBrainComponent()->StopLogic(TEXT("Dead"));
        SetActorTickEnabled(false);
        return;
    }

}
