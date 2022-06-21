// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

class UHealth;

UCLASS()
class FPSSURVIVAL_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

	UHealth* HealthComp;

public:
	// Sets default values for this character's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bIsAlive = true;
	bool bIsStunned = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void Attack(float Range, float Damage, float Radius);

	UFUNCTION(BlueprintPure)
	bool IsAlive() const;

	UFUNCTION(BlueprintPure)
	bool IsStunned() const;

private:

	

};
