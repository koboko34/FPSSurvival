// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

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

	if (GetOwner() == nullptr)
	{
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Owner: %s"), *GetOwner()->GetActorNameOrLabel());
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	TArray<AActor*> ActorsToIgnore;
	UGameplayStatics::ApplyRadialDamageWithFalloff(this, MaxDamage, MinDamage, GetActorLocation(), InnerRadius, OuterRadius, DamageFalloff, UDamageType::StaticClass(), ActorsToIgnore, this);
	DrawDebugSphere(GetWorld(), GetActorLocation(), InnerRadius, 16, FColor::Purple, false, 5);
	DrawDebugSphere(GetWorld(), GetActorLocation(), InnerRadius + OuterRadius, 16, FColor::Blue, false, 5);
	
	DestroyProjectile();
}

void AProjectile::DestroyProjectile()
{
	Destroy();
}

