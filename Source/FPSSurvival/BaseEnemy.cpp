// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Health.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "MaxAmmo.h"
#include "Math/UnrealMathUtility.h"
#include "SurvivalGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComp = CreateDefaultSubobject<UHealth>(TEXT("Health Component"));

}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	SurvivalGameMode = Cast<ASurvivalGameMode>(UGameplayStatics::GetGameMode(this));

	ClearStunDelegate.BindUObject(this, &ABaseEnemy::ClearStun);
	StartExitStunDelegate.BindUObject(this, &ABaseEnemy::StartExitStun);	
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ABaseEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(HealthComp->GetHealth(), DamageToApply);
	DamageToApply = FMath::RoundHalfFromZero(DamageToApply);
	HealthComp->SetHealth(HealthComp->GetHealth() - DamageToApply);

	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), HealthComp->GetHealth());

	if (HealthComp->GetHealth() <= 0)
	{
		HandleDeath();
	}

	SurvivalGameMode->AddPoints(10);

	return DamageToApply;
}

void ABaseEnemy::HandleDeath()
{
	bIsAlive = false;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetEnableGravity(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetEnableGravity(false);
	SurvivalGameMode->AddPoints(100);

	if (FMath::RandRange(0, 100) < 25)
	{
		if (MaxAmmoClass)
		{
			GetWorld()->SpawnActor<AMaxAmmo>(MaxAmmoClass, GetActorLocation(), GetActorRotation());
		}
	}
}

void ABaseEnemy::Attack(float Range, float Damage, float Radius)
{
	TArray<FHitResult> HitArray;
	FVector Start = GetMesh()->GetComponentLocation();
	FVector End = GetMesh()->GetRightVector() * Range;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	GetWorld()->SweepMultiByChannel(
		HitArray,
		Start,
		End,
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(Radius),
		Params
	);

	int i = 0;
	while (i < HitArray.Num())
	{
		// UE_LOG(LogTemp, Warning, TEXT("%s"), *HitArray[i].GetActor()->GetActorNameOrLabel());
		AShooterCharacter* HitActor = Cast<AShooterCharacter>(HitArray[i].GetActor());

		if (HitActor != nullptr && HitActor->bIsPlayerVisible == true)
		{
			UGameplayStatics::ApplyDamage(HitActor, Damage, GetController(), this, UDamageType::StaticClass());
			break;
		}
		i++;
	}			
}

void ABaseEnemy::ApplyStun(float Duration)
{
	bIsStunned = true;
	GetWorldTimerManager().SetTimer(StartExitStunHandle, StartExitStunDelegate, Duration, false);
}

void ABaseEnemy::StartExitStun()
{
	bCanExitStun = true;
	GetWorldTimerManager().SetTimer(ClearStunHandle, ClearStunDelegate, 0.35, false);
}

void ABaseEnemy::ClearStun()
{
	bIsStunned = false;
	bCanExitStun = false;
}

bool ABaseEnemy::IsAlive() const
{
	return bIsAlive;
}

bool ABaseEnemy::IsStunned() const
{
	return bIsStunned;
}

bool ABaseEnemy::CanExitStun() const
{
	return bCanExitStun;
}


