// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGun.h"
#include "Rifle.generated.h"

class AShooterCharacter;
/**
 * 
 */
UCLASS()
class FPSSURVIVAL_API ARifle : public ABaseGun
{
	GENERATED_BODY()

public:
	ARifle();

protected:

	virtual void BeginPlay() override;

public:
	void PullTrigger(AShooterCharacter* PlayerCharacter);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	int Range = 20000;
	
};
