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
class ASurvivalGameMode;
class APortalPointer;

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
	void LookRightRate(float Val);
	void LookUpRate(float Val);
	void OnSprint();


	void OnAbilityOne();
	// void OnAbilityOneExit();
	void OnAbilityTwo();
	void OnAbilityUlt();

	void TogglePlayMode();

	AAbilityPortal* TracePortal(bool& bSuccess);
	void HandlePortalSpawn();
	void ClearPortalsRef();

	void AbilityTwoTick();

	void EndAbilityUlt();

	UFUNCTION()
	void SelectActiveGun(ABaseGun* newActiveGun);
	void ToggleActiveGun();
	bool IsSameGun(ABaseGun* SwapTargetGun);

	void HandleDeath();

	void OnInteract();
	void OnTogglePause();

	class AShooterPlayerController* ShooterController;

	UPROPERTY(EditDefaultsOnly, Category = "Gun", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseGun> PrimaryGunClass;
	ABaseGun* PrimaryGun;
	UPROPERTY(EditDefaultsOnly, Category = "Gun", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseGun> SecondaryGunClass;
	ABaseGun* SecondaryGun;

	ARifle* RifleGun;
	ALauncher* LauncherGun;

	UPROPERTY(EditDefaultsOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SprintSpeed = 1000;
	UPROPERTY(EditDefaultsOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float StrafeSpeed = 600;
	UPROPERTY(EditDefaultsOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float RotationRate = 600;
	float ControllerDeadzone = 0.05;

	bool bIsShootPressed = false;
	bool bIsSprinting = false;

	EPlayMode ActivePlayMode;
	float AbilityRange = 2000;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AAbilityPortal> AbilityPortalClass;
	UPROPERTY()
	AAbilityPortal* FirstPortal;
	UPROPERTY()
	AAbilityPortal* SecondPortal;
	UPROPERTY()
	TArray<AAbilityPortal*> PortalArray;
	
	bool bAbilityOneReady = true;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	float AbilityOneCooldown = 8;
	float AbilityOneCurrentCooldown = 0;
	FTimerHandle AbilityOneCooldownHandle;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APortalPointer> PortalPtrClass;
	UPROPERTY()
	APortalPointer* PortalPtr;
	
	bool bAbilityTwoReady = true;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	float AbilityTwoCooldown = 10;
	float AbilityTwoCurrentCooldown = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	float AbilityTwoDuration = 5;
	FTimerHandle AbilityTwoCooldownHandle;
	FTimerHandle AbilityTwoTickHandle;
	FTimerDelegate AbilityTwoTickDelegate;
	int TickLimit = 10;
	int CurrentTick = 0;
	float HealthPerTick = 5;
	bool bAbilityTwoActive = false;

	bool bAbilityUltReady = true;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	float AbilityUltCooldown = 15;
	float AbilityUltCurrentCooldown = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	float AbilityUltDuration = 5;
	FTimerHandle AbilityUltCooldownHandle;
	FTimerHandle AbilityUltDurationHandle;
	FTimerDelegate AbilityUltDurationDelegate;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	USoundBase* DeathSound;

	ASurvivalGameMode* SurvivalGameMode;

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
	bool GetAbilityTwoActive() const { return bAbilityTwoActive; }
	UFUNCTION(BlueprintPure)
	UHealth* GetHealthComponent() const { return HealthComp; }
	UFUNCTION(BlueprintPure)
	ALauncher* GetLauncher() const { return LauncherGun; }
	UFUNCTION(BlueprintPure)
	ABaseGun* GetActiveGun() const { return ActiveGun; }
	UFUNCTION(BlueprintPure)
	ABaseGun* GetInactiveGun() const { return InactiveGun; }
	UFUNCTION(BlueprintPure)
	bool IsAlive() const { return bIsAlive; }



	UFUNCTION(BlueprintImplementableEvent)
	void InteractTrace();
	
	void StopSprint();
	
	UFUNCTION(BlueprintCallable)
	void OnMaxAmmo();
	bool OnHealthUp(float HealthToAdd);

	UPROPERTY(BlueprintReadOnly)
	ABaseGun* ActiveGun;
	UPROPERTY(BlueprintReadOnly)
	ABaseGun* InactiveGun;

	UPROPERTY(EditAnywhere)
	bool bIsPlayerVisible = true;
	bool bIsAlive = true;
};

