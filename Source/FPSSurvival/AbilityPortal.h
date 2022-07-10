// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AbilityPortal.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class AAbilityProjectile;

UCLASS()
class FPSSURVIVAL_API AAbilityPortal : public APawn
{
	GENERATED_BODY()

	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* PortalParticleSystem;

public:
	// Sets default values for this pawn's properties
	AAbilityPortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UStaticMeshComponent* GetMesh() const;
	
	void SetupPortal(AAbilityPortal* Partner);

	UFUNCTION(BlueprintImplementableEvent)
	void HandleNiagaraSetup();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HandleNiagaraDestruction();

	void HandleDestruction();

private:

	UPROPERTY()
	AAbilityPortal* PartnerPortal;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* PortalComp;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AAbilityProjectile> AbilityProjectileClass;
	UPROPERTY()
	AAbilityProjectile* SpawnedProjectile;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float SpawnDelay = 1;
	
	void SpawnProjectile();


};
