// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseGun.generated.h"

UCLASS()
class FPSSURVIVAL_API ABaseGun : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* Muzzle;
	
public:	
	// Sets default values for this actor's properties
	ABaseGun();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "Gun")
	int Ammo;
	UPROPERTY(EditAnywhere, Category = "Gun")
	int MagSize = 30;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gun")
	int ReserveAmmo = 270;
	UPROPERTY(EditAnywhere, Category = "Gun")
	bool IsFullyAuto = true;
	UPROPERTY(EditAnywhere, Category = "Gun")
	float RPM = 600;
	float FireRate = 1 / (RPM / 60);
	UPROPERTY(EditAnywhere, Category = "Gun")
	float ReloadTime = 2;

	bool CanShoot();
	bool CanReload();

	bool GunTrace(FHitResult& OutHit, FVector Start, FVector End);

public:	
	// virtual void Tick(float DeltaTime) override;
	void StartShoot();
	void Shoot();
	void StopShoot();
	void StartReload();
	void Reload();
	void CancelReload();

	USkeletalMeshComponent* GetMesh() const;
	USceneComponent* GetMuzzle() const;
	// int GetAmmo() const;

private:

	class AShooterCharacter* PlayerCharacter;
	class ARifle* Rifle;
	class ALauncher* Launcher;

	FTimerHandle ShootHandle;
	FTimerDelegate ShootDelegate;

	FTimerHandle ReloadHandle;
	FTimerDelegate ReloadDelegate;

	

};
