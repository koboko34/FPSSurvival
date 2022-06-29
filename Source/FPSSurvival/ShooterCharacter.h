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
class ARifle;
class ALauncher;

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

	void EndAbilityUlt();

	UFUNCTION()
	void SelectActiveGun(ABaseGun* newActiveGun);
	void ToggleActiveGun();
	bool IsSameGun(ABaseGun* SwapTargetGun);

	void HandleDeath();

	void OnInteract();


	UPROPERTY(EditDefaultsOnly, Category = "Gun", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseGun> PrimaryGunClass;
	ABaseGun* PrimaryGun;
	UPROPERTY(EditDefaultsOnly, Category = "Gun", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseGun> SecondaryGunClass;
	ABaseGun* SecondaryGun;

	ARifle* RifleGun;
	ALauncher* LauncherGun;

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

	bool bAbilityTwoReady = true;
	float AbilityTwoCooldown = 10;
	float AbilityTwoCurrentCooldown = 0;
	FTimerHandle AbilityTwoCooldownHandle;

	bool bAbilityUltReady = true;
	float AbilityUltCooldown = 15;
	float AbilityUltCurrentCooldown = 0;
	float AbilityUltDuration = 5;
	FTimerHandle AbilityUltCooldownHandle;
	FTimerHandle AbilityUltDurationHandle;
	FTimerDelegate AbilityUltDurationDelegate;

public:
	virtual void Tick(float DeltaTime) override;
	
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION(BlueprintPure)
	float GetAbilityOneCooldown() const { return AbilityOneCurrentCooldown; }
	UFUNCTION(BlueprintPure)
	float GetAbilityTwoCooldown() const { return AbilityTwoCurrentCooldown; }
	UFUNCTION(BlueprintPure)
	float GetAbilityUltCooldown() const { return AbilityUltCurrentCooldown; }
	UFUNCTION(BlueprintPure)
	UHealth* GetHealthComponent() const { return HealthComp; }
	ALauncher* GetLauncher() const { return LauncherGun; }

	UFUNCTION(BlueprintImplementableEvent)
	void InteractTrace();
	
	void StopSprint();

	void OnMaxAmmo();

	UPROPERTY(BlueprintReadOnly)
	ABaseGun* ActiveGun;
	UPROPERTY(BlueprintReadOnly)
	ABaseGun* InactiveGun;

	UPROPERTY(EditAnywhere)
	bool bIsPlayerVisible = true;
};

