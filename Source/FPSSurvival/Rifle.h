// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGun.h"
#include "Rifle.generated.h"

class AShooterCharacter;
class USoundBase;
class UParticleSystem;

UCLASS()
class FPSSURVIVAL_API ARifle : public ABaseGun
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* MuzzleFlashParticleComp;

public:
	ARifle();

protected:

	virtual void BeginPlay() override;

public:
	void PullTrigger(AShooterCharacter* PlayerCharacter);

	void ShowMuzzleFlash();
	void HideMuzzleFlash();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float Range = 20000;
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	int TargetPenCount = 5;
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactParticle;
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	USoundBase* FleshImpactSound;
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	USoundBase* ConcreteImpactSound;
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	USoundBase* ShotSound;

};
