// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SurvivalGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FPSSURVIVAL_API ASurvivalGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	int GetPoints() const;
	int AddPoints(int PointsToAdd);
	UFUNCTION(BlueprintCallable)
	int RemovePoints(int PointsToRemove);

protected:

private:

	int Points = 1000;
	
};
