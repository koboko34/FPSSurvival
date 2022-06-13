// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityPortal.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
AAbilityPortal::AAbilityPortal()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(Root);
	PortalParticles = CreateDefaultSubobject<UNiagaraComponent>("Portal Particles");
	PortalParticles->SetupAttachment(Mesh);
	
}

// Called when the game starts or when spawned
void AAbilityPortal::BeginPlay()
{
	Super::BeginPlay();

	Mesh->SetVisibility(false);	
}

// Called every frame
void AAbilityPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAbilityPortal::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAbilityPortal::SetupPortal(AAbilityPortal* Partner)
{
	PartnerPortal = Partner;

	UE_LOG(LogTemp, Warning, TEXT("%s's partner: %s"), *GetActorNameOrLabel(), *PartnerPortal->GetActorNameOrLabel());
	
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PartnerPortal->GetActorLocation());
	FRotator AdjustedLookAtRotation = FRotator(0, LookAtRotation.Yaw, 0);

	SetActorRotation(AdjustedLookAtRotation);

	Mesh->SetVisibility(true);
}

