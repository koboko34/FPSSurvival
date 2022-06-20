// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetInRangeFalse.generated.h"

/**
 * 
 */
UCLASS()
class FPSSURVIVAL_API UBTTask_SetInRangeFalse : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_SetInRangeFalse();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
