// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"
#include "Components/BoxComponent.h"
#include "Mover.h"
#include "TimerManager.h"

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(RootComponent);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetBoxExtent(FVector(100, 100, 100));
	MoverComp = CreateDefaultSubobject<UMover>("Mover");

}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();

	DestroyDelegate = FTimerDelegate::CreateUObject(this, &AObstacle::DestroyActor);
	
}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AObstacle::DestroyAfterMove()
{
	if (bDestroyAfterMove)
	{
		GetWorldTimerManager().SetTimer(DestroyHandle, DestroyDelegate, MoverComp->GetMoveTime(), false);
	}
}

void AObstacle::DestroyActor()
{
	Destroy();
}



