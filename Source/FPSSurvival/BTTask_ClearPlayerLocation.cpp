// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearPlayerLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearPlayerLocation::UBTTask_ClearPlayerLocation()
{
    NodeName = TEXT("Clear Player Location");
}

EBTNodeResult::Type UBTTask_ClearPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    OwnerComp.GetAIOwner()->GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
    return EBTNodeResult::Succeeded;
}
