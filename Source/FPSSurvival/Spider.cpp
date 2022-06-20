// Fill out your copyright notice in the Description page of Project Settings.


#include "Spider.h"
#include "Math/UnrealMathUtility.h"


void ASpider::BeginPlay()
{
    Super::BeginPlay();

    // AttackAnims.Add(AttackAnim1);
    // AttackAnims.Add(AttackAnim2);
    // AttackAnims.Add(AttackAnim3);
}

void ASpider::PlayAttackAnim()
{
    int Min = 1;
    int Max = 3;
    int i = FMath::RandRange(Min, Max);

    if (i == 1)
    {
        PlayAnimMontage(AttackAnim1);
        return;
    }
    if (i == 2)
    {
        PlayAnimMontage(AttackAnim2);
        return;
    }
    if (i == 3)
    {
        PlayAnimMontage(AttackAnim3);
        return;
    }
}

void ASpider::SetInRange(bool NewBool)
{
    bInRange = NewBool;
}

bool ASpider::GetInRange() const
{
    return bInRange;
}