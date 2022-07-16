// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePickUp.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"

// Sets default values
ABasePickUp::ABasePickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	SetRootComponent(Capsule);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(Capsule);

}

// Called when the game starts or when spawned
void ABasePickUp::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle Handle;
	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &ABasePickUp::DestroySelf);
	GetWorldTimerManager().SetTimer(Handle, Delegate, 30, false);

	PlayerCharacter = Cast<AShooterCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (PlayerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter not set in %s"), *GetActorNameOrLabel());
	}
}

// Called every frame
// void ABasePickUp::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);

// }

void ABasePickUp::DestroySelf()
{
	Destroy();
}

