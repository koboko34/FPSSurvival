// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAIController.h"
#include "SpiderAIController.generated.h"

/**
 * 
 */
UCLASS()
class FPSSURVIVAL_API ASpiderAIController : public ABaseAIController
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* AIBehavior;

	class ASpider* Spider;

	class AShooterCharacter* Player;
};
