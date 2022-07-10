// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalGameMode.h"
#include "BaseEnemy.h"
#include "Spider.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

void ASurvivalGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (CustomStartRound == true)
    {
        Round--;
    }
    else
    {
        Round = 0;
    }

    NextRoundDelegate = FTimerDelegate::CreateUObject(this, &ASurvivalGameMode::NextRound);
    PrepareNextRound();

    // StatsDelegate = FTimerDelegate::CreateUObject(this, &ASurvivalGameMode::ShowStats);
    // GetWorldTimerManager().SetTimer(StatsHandle, StatsDelegate, 1, true);
}

int ASurvivalGameMode::GetPoints() const
{
    return Points;
}

int ASurvivalGameMode::AddPoints(int PointsToAdd)
{
    Points = Points + PointsToAdd;
    return Points;
}

int ASurvivalGameMode::RemovePoints(int PointsToRemove)
{
    Points = Points - PointsToRemove;
    return Points;
}

void ASurvivalGameMode::AddDeadEnemy(ABaseEnemy* DeadEnemy)
{
    DeadEnemies.Add(DeadEnemy);

    if (DeadEnemies.Num() >= EnemyClearCount)
    {
        ClearDeadEnemies();
    }

    // UE_LOG(LogTemp, Warning, TEXT("%i"), DeadEnemies.Num());
}

void ASurvivalGameMode::ClearDeadEnemies()
{
    for (int i = 0; i < 20; i++)
    {
        DeadEnemies[0]->Destroy();
        DeadEnemies.RemoveAt(0);
    }

    UE_LOG(LogTemp, Warning, TEXT("Cleared dead enemies"));
}

void ASurvivalGameMode::StartRound(int RoundToStart)
{
    SpidersKilled = 0;
    SpidersSpawned = 0;
    SpidersToSpawn = RoundToStart * 10;

    if (RoundStartSound)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), RoundStartSound);
    }

    UE_LOG(LogTemp, Warning, TEXT("Starting Round %i"), Round);
}

void ASurvivalGameMode::PrepareNextRound()
{
    GetWorldTimerManager().SetTimer(NextRoundHandle, NextRoundDelegate, 5, false);
}

void ASurvivalGameMode::EndGame(bool bIsPlayerWinner)
{
    bGameHasEnded = true;

    FTimerHandle Handle;
    GetWorldTimerManager().SetTimer(Handle, this, &ASurvivalGameMode::RestartLevel, 5, false);
}

void ASurvivalGameMode::NextRound()
{
    Round++;
    StartRound(Round);
}

int ASurvivalGameMode::AddToSpiderCount(bool bIsValid)
{
    if (bIsValid)
    {
        SpidersSpawned++;
        SpidersAlive++;
    }

    return SpidersAlive;
}

int ASurvivalGameMode::RemoveFromSpiderCount()
{
    SpidersAlive--;
    SpidersKilled++;

    if (SpidersKilled == SpidersToSpawn)
    {
        PrepareNextRound();
        UE_LOG(LogTemp, Warning, TEXT("Round ended. Preparing next round!"));
    }
    return SpidersAlive;
}

bool ASurvivalGameMode::ShouldSpawn() const
{
    if (SpidersToSpawn > SpidersSpawned && SpidersAlive < SpiderLimit)
    {
        return true;
    }
    return false;
}

bool ASurvivalGameMode::SpawnSpider(FVector SpawnLocation)
{
    if (SpiderClass == nullptr)
    {
        return false;
    }
    
    ASpider* SpawnedSpider = GetWorld()->SpawnActor<ASpider>(
        SpiderClass,
        SpawnLocation,
        FRotator::ZeroRotator
    );

    if (SpawnedSpider == nullptr)
    {
        return false;
    }

    AddToSpiderCount(true);

    return true;
}

void ASurvivalGameMode::ShowStats()
{
    UE_LOG(LogTemp, Warning, TEXT("SpidersToSpawn: %i"), SpidersToSpawn);
    UE_LOG(LogTemp, Warning, TEXT("SpidersSpawned: %i"), SpidersSpawned);
    UE_LOG(LogTemp, Warning, TEXT("SpidersKilled: %i"), SpidersKilled);
}

void ASurvivalGameMode::RestartLevel()
{
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}