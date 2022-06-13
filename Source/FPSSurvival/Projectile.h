// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"


UCLASS()
class FPSSURVIVAL_API AProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	class UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	class UProjectileMovementComponent* ProjectileMovementComponent;
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float TravelSpeed = 5000;

	UFUNCTION()
	void DestroyProjectile();

};
