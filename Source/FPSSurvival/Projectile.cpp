// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->InitialSpeed = TravelSpeed;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AProjectile::DestroyProjectile);
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 10, false);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	DestroyProjectile();
}

void AProjectile::DestroyProjectile()
{
	UE_LOG(LogTemp, Warning, TEXT("Boom!"));
	Destroy();
}

