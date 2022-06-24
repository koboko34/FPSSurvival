// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ClearPlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class FPSSURVIVAL_API UBTTask_ClearPlayerLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UBTTask_ClearPlayerLocation();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
};
