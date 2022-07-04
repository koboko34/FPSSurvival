// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnPoint.h"

// Sets default values
ASpawnPoint::ASpawnPoint()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

}

// Called when the game starts or when spawned
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

