// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayMode.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class ABaseGun;
class AAbilityPortal;
class UHealth;

UCLASS(config=Game)
class AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditDefaultsOnly)
	UHealth* HealthComp;

public:
	AShooterCharacter();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	
protected:
	virtual void BeginPlay();

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;


private:
	void OnShootPressed();
	void OnShootReleased();
	void OnReload();
	void MoveForward(float Val);
	void MoveRight(float Val);
	void OnSprint();

	void OnAbilityOne();
	void OnAbilityOneExit();
	void OnAbilityTwo();
	void OnAbilityUlt();

	void TogglePlayMode();

	AAbilityPortal* TracePortal(bool& bSuccess);
	void HandlePortalSpawn();
	void ClearPortalsRef();

	UFUNCTION()
	void SelectActiveGun(ABaseGun* newActiveGun);
	void ToggleActiveGun();
	bool IsSameGun(ABaseGun* SwapTargetGun);

	void HandleDeath();

	UPROPERTY(EditDefaultsOnly, Category = "Gun", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseGun> PrimaryGunClass;
	ABaseGun* PrimaryGun;
	UPROPERTY(EditDefaultsOnly, Category = "Gun", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseGun> SecondaryGunClass;
	ABaseGun* SecondaryGun;

	UPROPERTY(EditDefaultsOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SprintSpeed = 1200;
	UPROPERTY(EditDefaultsOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float StrafeSpeed = 600;

	bool bIsShootPressed = false;
	bool bIsSprinting = false;

	EPlayMode ActivePlayMode;
	float AbilityRange = 2000;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AAbilityPortal> AbilityPortalClass;
	AAbilityPortal* FirstPortal;
	AAbilityPortal* SecondPortal;
	float AbilityOneCooldown = 8;
	float AbilityOneCurrentCooldown = 0;
	FTimerHandle AbilityOneCooldownHandle;
	bool bAbilityOneReady = true;

public:
	virtual void Tick(float DeltaTime) override;
	
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION(BlueprintPure)
	float GetAbilityOneCooldown() const { return AbilityOneCurrentCooldown; }
	UFUNCTION(BlueprintPure)
	UHealth* GetHealthComponent() const { return HealthComp; }
	
	void StopSprint();

	UPROPERTY(BlueprintReadOnly)
	ABaseGun* ActiveGun;
	UPROPERTY(BlueprintReadOnly)
	ABaseGun* InactiveGun;
};

