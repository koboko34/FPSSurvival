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
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "Gun")
	int ReserveAmmo;
	UPROPERTY(EditAnywhere, Category = "Gun")
	int MaxReserveAmmo = 270;
	UPROPERTY(EditAnywhere, Category = "Gun")
	bool IsFullyAuto = true;
	UPROPERTY(EditAnywhere, Category = "Gun")
	float RPM = 600;
	float FireRate = 1 / (RPM / 60);
	UPROPERTY(EditAnywhere, Category = "Gun")
	float ReloadTime = 2;
	UPROPERTY(EditAnywhere, Category = "Gun")
	float Damage = 10;

	bool CanShoot();
	bool CanReload();

	bool GunTrace(TArray<FHitResult> &HitActorArray, FVector Start, FVector End);

	class AShooterCharacter* PlayerCharacter;

public:	
	// virtual void Tick(float DeltaTime) override;
	void StartShoot();
	void Shoot();
	void StopShoot();
	void StartReload();
	void Reload();
	void CancelReload();
	void OnMaxAmmo();

	float GetDamage() const { return Damage; }
	UFUNCTION(BlueprintPure)
	int GetAmmo() const { return Ammo; }
	UFUNCTION(BlueprintPure)
	int GetReserveAmmo() const { return ReserveAmmo; }

	UFUNCTION(BlueprintPure)
	float ReloadProgress() const;

	USkeletalMeshComponent* GetMesh() const;
	USceneComponent* GetMuzzle() const;
	// int GetAmmo() const;

private:

	class ARifle* Rifle;
	class ALauncher* Launcher;

	FTimerHandle ShootHandle;
	FTimerDelegate ShootDelegate;

	FTimerHandle ReloadHandle;
	FTimerDelegate ReloadDelegate;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsReloading = false;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzleFlash;

};
