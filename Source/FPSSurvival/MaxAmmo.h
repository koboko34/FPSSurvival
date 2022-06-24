// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickUp.h"
#include "MaxAmmo.generated.h"

/**
 * 
 */
UCLASS()
class FPSSURVIVAL_API AMaxAmmo : public ABasePickUp
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;
};