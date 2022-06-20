// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetInRangeTrue.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_SetInRangeTrue::UBTTask_SetInRangeTrue()
{
    NodeName = TEXT("Set InRange True");
}

EBTNodeResult::Type UBTTask_SetInRangeTrue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool(TEXT("InRange"), true);
    
    return EBTNodeResult::Succeeded;
}

