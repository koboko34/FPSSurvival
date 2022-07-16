// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePickUp.generated.h"

UCLASS()
class FPSSURVIVAL_API ABasePickUp : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* Capsule;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	
public:	
	// Sets default values for this actor's properties
	ABasePickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* SoundEffect;

	class AShooterCharacter* PlayerCharacter;

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;

	UStaticMeshComponent* GetMesh() { return Mesh; }
	UCapsuleComponent* GetCapsuleComponent() { return Capsule; }

private:

	void DestroySelf();
};
