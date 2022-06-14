// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityProjectile.h"
// #include "GameFramework/ProjectileMovementComponent.h"
#include "Math/UnrealMathVectorCommon.h"
#include "AbilityPortal.h"

// Sets default values
AAbilityProjectile::AAbilityProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(Root);

	// ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement");
	// ProjectileMovementComponent->InitialSpeed = 100;
	// ProjectileMovementComponent->ProjectileGravityScale = 0;

}

// Called when the game starts or when spawned
void AAbilityProjectile::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	
}

// Called every frame
void AAbilityProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeElapsed = TimeElapsed + DeltaTime;

	if (TargetIsSet)
	{
		SetActorLocation(FMath::Lerp(StartLocation, TargetLocation, (TimeElapsed / LerpDuration)));
		if (TimeElapsed >= LerpDuration)
		{
			AAbilityPortal* PortalOwner = Cast<AAbilityPortal>(GetOwner());
			if (PortalOwner)
			{
				PortalOwner->HandleDestruction();
			}
			Destroy();
		}
	}

}

void AAbilityProjectile::SetTargetLocation(FVector NewTargetLocation)
{
	TargetLocation = NewTargetLocation;
	TargetIsSet = true;
}

