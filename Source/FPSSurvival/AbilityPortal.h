// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AbilityPortal.generated.h"

class UNiagaraComponent;

UCLASS()
class FPSSURVIVAL_API AAbilityPortal : public APawn
{
	GENERATED_BODY()

	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly)
	UNiagaraComponent* PortalParticles;

public:
	// Sets default values for this pawn's properties
	AAbilityPortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void SetupPortal(AAbilityPortal* Partner);

private:

	AAbilityPortal* PartnerPortal;


};
