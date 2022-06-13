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

public:
	AShooterCharacter();

	
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
	void OnAbilityTwo();
	void OnAbilityUlt();

	void TogglePlayMode();

	AAbilityPortal* TracePortal(bool& bSuccess);
	void ClearPortalsRef();

	UFUNCTION()
	void SelectActiveGun(ABaseGun* newActiveGun);
	void ToggleActiveGun();
	bool IsSameGun(ABaseGun* SwapTargetGun);

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
	float AbilityRange = 1000;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AAbilityPortal> AbilityPortalClass;
	AAbilityPortal* FirstPortal;
	AAbilityPortal* SecondPortal;

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	
	void StopSprint();

	UPROPERTY(BlueprintReadOnly)
	ABaseGun* ActiveGun;
	UPROPERTY(BlueprintReadOnly)
	ABaseGun* InactiveGun;
};

