// Fill out your copyright notice in the Description page of Project Settings.


#include "Health.h"

UHealth::UHealth()
{

}


void UHealth::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	
}


// void UHealth::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

// 	// ...
// }

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

