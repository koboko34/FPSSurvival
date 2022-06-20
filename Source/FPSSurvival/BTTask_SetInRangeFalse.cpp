// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetInRangeFalse.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetInRangeFalse::UBTTask_SetInRangeFalse()
{
    NodeName = TEXT("Set InRange False");
}

EBTNodeResult::Type UBTTask_SetInRangeFalse::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool(TEXT("InRange"), false);

    return EBTNodeResult::Succeeded;
}
