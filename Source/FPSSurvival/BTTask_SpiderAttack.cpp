// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SpiderAttack.h"
#include "AIController.h"
#include "Spider.h"

UBTTask_SpiderAttack::UBTTask_SpiderAttack()
{
    NodeName = TEXT("Spider Attack");
}

EBTNodeResult::Type UBTTask_SpiderAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    ASpider* Spider = Cast<ASpider>(OwnerComp.GetAIOwner()->GetPawn());

    if (Spider == nullptr)
    {
        EBTNodeResult::Failed;
    }

    Spider->PlayAttackAnim();

    return EBTNodeResult::Succeeded;
}

