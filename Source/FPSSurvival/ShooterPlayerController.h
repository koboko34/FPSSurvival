// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInfoDelegate, FString, Message);

UCLASS()
class FPSSURVIVAL_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

	AShooterPlayerController();

protected:

	virtual void BeginPlay() override;

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> HUDClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* HUD;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> BloodEffectClass;

	UPROPERTY()
	UUserWidget* BloodEffect;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> DeathEffectClass;

	UPROPERTY()
	UUserWidget* DeathEffect;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> PauseMenuClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* PauseMenu;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> AbilityUltClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* AbilityUlt;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* HitmarkerSound;

	UPROPERTY(BlueprintAssignable, Category = "HUD Info");
	FInfoDelegate OnInfoDelegate;

	UPROPERTY(BlueprintReadOnly)
	FString Message = TEXT("This is a test message");

	void ShowInfo(FString MessageToShow);

	void ShowPauseMenu();
	void HidePauseMenu();

	void ShowAbilityUlt();
	void HideAbilityUlt();

	void ShowBloodEffect();
	void ShowDeathEffect();

	UFUNCTION(BlueprintPure)
	bool GetMakeHitmarkerVisible() const { return bMakeHitmarkerVisible; }
	void ShowHitmarker();
	void HideHitmarker();

	UFUNCTION(BlueprintPure)
	bool GetMakeKillmarkerVisible() const { return bMakeKillmarkerVisible; }
	void ShowKillmarker();
	void HideKillmarker();

private:

	void SetBloodEffectNull();

	bool bMakeHitmarkerVisible = false;
	FTimerHandle HitmarkerHandle;

	bool bMakeKillmarkerVisible = false;
	FTimerHandle KillmarkerHandle;

	FTimerDelegate BloodEffectDelegate;
};
