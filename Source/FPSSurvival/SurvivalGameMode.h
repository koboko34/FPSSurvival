// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SurvivalGameMode.generated.h"

class ABaseEnemy;
class ASpider;

UCLASS()
class FPSSURVIVAL_API ASurvivalGameMode : public AGameModeBase
{
	GENERATED_BODY()

	ASurvivalGameMode();

public:

	UFUNCTION(BlueprintPure)
	int GetPoints() const;
	int AddPoints(int PointsToAdd);
	UFUNCTION(BlueprintCallable)
	int RemovePoints(int PointsToRemove);

	void AddDeadEnemy(ABaseEnemy* DeadEnemy);

	UPROPERTY(BlueprintReadWrite)
	int SpidersAlive = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int SpiderLimit = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int Round = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int SpidersToSpawn = 0;
	UPROPERTY(BlueprintReadWrite)
	int SpidersSpawned = 0;
	UPROPERTY(BlueprintReadWrite)
	int SpidersKilled = 0;

	UFUNCTION(BlueprintCallable)
	void StartRound(int RoundToStart);
	UFUNCTION(BlueprintCallable)
	void NextRound();

	UFUNCTION(BlueprintCallable)
	bool SpawnSpider(FVector SpawnLocation);

	UFUNCTION(BlueprintCallable)
	int AddToSpiderCount(bool bIsValid);
	UFUNCTION(BlueprintCallable)
	int RemoveFromSpiderCount();

	UFUNCTION(BlueprintPure)
	bool ShouldSpawn() const;

	void PrepareNextRound();

	UPROPERTY(EditAnywhere)
	bool CustomStartRound = false;

	UFUNCTION()
	void EndGame(bool bIsPlayerWinner);

	UFUNCTION(BlueprintPure)
	int GetRound() const { return Round; }
	UFUNCTION(BlueprintPure)
	bool GameHasEnded() const { return bGameHasEnded; }

protected:

	virtual void BeginPlay() override;

private:

	void ClearDeadEnemies();

	int Points = 0;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASpider> SpiderClass;

	int EnemyClearCount = 25;
	TArray<ABaseEnemy*> DeadEnemies;

	FTimerHandle NextRoundHandle;
	FTimerDelegate NextRoundDelegate;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	USoundBase* RoundStartSound;

	void ShowStats();
	FTimerHandle StatsHandle;
	FTimerDelegate StatsDelegate;

	bool bGameHasEnded = false;

	void RestartLevel();
	
};
