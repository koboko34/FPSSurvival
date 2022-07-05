// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FPSSURVIVAL_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

	// AShooterPlayerController();

protected:

	virtual void BeginPlay() override;

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> HUDClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* HUD;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> PauseMenuClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* PauseMenu;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> AbilityUltClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* AbilityUlt;

	void ShowPauseMenu();
	void HidePauseMenu();

	void ShowAbilityUlt();
	void HideAbilityUlt();

};
