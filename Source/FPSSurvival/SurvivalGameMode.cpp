// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalGameMode.h"


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
