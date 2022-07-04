// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

class UHealth;
class AMaxAmmo;
class AHealthUp;

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

	bool bCanExitStun = false;

	void HandleDeath();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	virtual void Attack(float Range, float Damage, float Radius);

	UFUNCTION()
	void ApplyStun(float Duration);
	UFUNCTION()
	void ClearStun();
	FTimerHandle ClearStunHandle;
	FTimerDelegate ClearStunDelegate;
	UFUNCTION()
	void StartExitStun();
	FTimerHandle StartExitStunHandle;
	FTimerDelegate StartExitStunDelegate;

	UFUNCTION(BlueprintPure)
	bool IsAlive() const;
	UFUNCTION(BlueprintPure)
	bool IsStunned() const;
	UFUNCTION(BlueprintPure)
	bool CanExitStun() const;	

private:

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMaxAmmo> MaxAmmoClass;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AHealthUp> HealthUpClass;

	class ASurvivalGameMode* SurvivalGameMode;
};
