// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGun.h"
#include "Launcher.generated.h"

class AShooterCharacter;
class AProjectile;
/**
 * 
 */
UCLASS()
class FPSSURVIVAL_API ALauncher : public ABaseGun
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystemComponent* MuzzleFlashParticle;

public:
	ALauncher();

protected:
	virtual void BeginPlay() override;

public:

	void PullTrigger(AShooterCharacter* PlayerCharacter);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectile> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	int Range = 20000;

	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	USoundBase* LauncherShotSound;

	FTimerDelegate MuzzleDelegate;

	void ShowMuzzleFlash();
	void HideMuzzleFlash();


};
