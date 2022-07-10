// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "Spider.generated.h"

class UAnimMontage;

UCLASS()
class FPSSURVIVAL_API ASpider : public ABaseEnemy
{
	GENERATED_BODY()

public:

	ASpider();

protected:

	virtual void BeginPlay() override;

public:

	void PlayAttackAnim();
	void SetInRange(bool NewBool);
	bool GetInRange() const;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackAnim1;
	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackAnim2;
	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackAnim3;

	bool bInRange = false;

	// TArray<UAnimMontage> AttackAnims;

};
