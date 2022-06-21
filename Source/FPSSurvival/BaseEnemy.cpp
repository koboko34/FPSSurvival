// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Health.h"

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
	
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

		if (HitActor != nullptr)
		{
			UGameplayStatics::ApplyDamage(HitActor, Damage, GetController(), this, UDamageType::StaticClass());
			break;
		}
		i++;
	}			
}

bool ABaseEnemy::IsAlive() const
{
	return bIsAlive;
}

bool ABaseEnemy::IsStunned() const
{
	return bIsStunned;
}


