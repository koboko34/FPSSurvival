// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetInRangeTrue.generated.h"

/**
 * 
 */
UCLASS()
class FPSSURVIVAL_API UBTTask_SetInRangeTrue : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_SetInRangeTrue();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
