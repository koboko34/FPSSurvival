// Fill out your copyright notice in the Description page of Project Settings.


#include "Health.h"

UHealth::UHealth()
{
	PrimaryComponentTick.bCanEverTick =  false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}


void UHealth::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
}


float UHealth::GetHealth() const
{
	return Health;
}

float UHealth::GetMaxHealth() const
{
	return MaxHealth;
}

void UHealth::SetHealth(float NewHealth)
{
	Health = NewHealth;
}

void UHealth::SetMaxHealth(float NewMaxHealth)
{
	MaxHealth = NewMaxHealth;
}