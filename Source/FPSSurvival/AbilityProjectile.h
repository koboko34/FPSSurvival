// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityProjectile.generated.h"

class ABaseEnemy;

UCLASS()
class FPSSURVIVAL_API AAbilityProjectile : public AActor
{
	GENERATED_BODY()

	USceneComponent* Root;
	UPROPERTY(EditInstanceOnly)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditInstanceOnly)
	class UCapsuleComponent* Capsule;
	// UPROPERTY(EditAnywhere)
	// UProjectileMovementComponent* ProjectileMovementComponent;

	
public:	
	// Sets default values for this actor's properties
	AAbilityProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetTargetLocation(FVector NewTargetLocation);

private:

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FVector StartLocation;
	FVector TargetLocation;

	bool TargetIsSet = false;

	float TimeElapsed = 0;
	UPROPERTY(EditAnywhere)
	float LerpDuration = 0.5;

	UPROPERTY(EditDefaultsOnly)
	float Damage = 25;
	UPROPERTY(EditDefaultsOnly)
	float StunDuration = 5;

	TArray<ABaseEnemy*> HitEnemyArray;

};
