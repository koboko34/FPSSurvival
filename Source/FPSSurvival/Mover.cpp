// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"


// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetOwner()->GetActorLocation();
	
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bShouldMove)
	{
		float Speed = FVector::Distance(StartLocation, StartLocation + MoveVector) / MoveTime;
		GetOwner()->SetActorLocation(FMath::VInterpConstantTo(GetOwner()->GetActorLocation(), StartLocation + MoveVector, DeltaTime, Speed));
	}
}

void UMover::Move()
{
	bShouldMove = true;
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &UMover::HandleFinish, MoveTime + 1, false);
}

void UMover::HandleFinish()
{
    SetComponentTickEnabled(false);
}