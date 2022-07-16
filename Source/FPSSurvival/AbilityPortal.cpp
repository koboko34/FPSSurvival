// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityPortal.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "AbilityProjectile.h"
#include "TimerManager.h"

// Sets default values
AAbilityPortal::AAbilityPortal()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(Root);
}

void AAbilityPortal::BeginPlay()
{
	Super::BeginPlay();

	Mesh->SetVisibility(false);
}

void AAbilityPortal::SetupPortal(AAbilityPortal* Partner)
{
	PartnerPortal = Partner;

	// UE_LOG(LogTemp, Warning, TEXT("%s's partner: %s"), *GetActorNameOrLabel(), *PartnerPortal->GetActorNameOrLabel());
	
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PartnerPortal->GetActorLocation());
	FRotator AdjustedLookAtRotation = FRotator(0, LookAtRotation.Yaw, 0);

	SetActorRotation(AdjustedLookAtRotation);

	HandleNiagaraSetup();

	Mesh->SetVisibility(true);

	FTimerHandle SpawnHandle;
	FTimerDelegate SpawnDelegate = FTimerDelegate::CreateUObject(this, &AAbilityPortal::SpawnProjectile);

	GetWorldTimerManager().SetTimer(SpawnHandle, SpawnDelegate, SpawnDelay, false);
	
}

UStaticMeshComponent* AAbilityPortal::GetMesh() const
{
	return Mesh;
}

void AAbilityPortal::SpawnProjectile()
{
	if (AbilityProjectileClass == nullptr)
	{
		return;
	}

	SpawnedProjectile = GetWorld()->SpawnActor<AAbilityProjectile>
	(
		AbilityProjectileClass,
		Mesh->GetComponentLocation(),
		GetActorRotation()
	);

	SpawnedProjectile->SetOwner(this);
	SpawnedProjectile->SetTargetLocation(PartnerPortal->GetMesh()->GetComponentLocation());
}

void AAbilityPortal::HandleDestruction()
{
	// UE_LOG(LogTemp, Warning, TEXT("HandleDestruction() called"));
	HandleNiagaraDestruction();
	Destroy();
}

