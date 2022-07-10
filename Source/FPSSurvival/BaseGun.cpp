// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGun.h"
#include "ShooterCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Rifle.h"
#include "Launcher.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseGun::ABaseGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(Mesh);


}

// Called when the game starts or when spawned
void ABaseGun::BeginPlay()
{
	Super::BeginPlay();

	OnMaxAmmo();

	ShootDelegate.BindUObject(this, &ABaseGun::Shoot);
	ReloadDelegate.BindUObject(this, &ABaseGun::Reload);
	
}

void ABaseGun::Shoot()
{
	
	if (CanShoot())
	{
		if (GetOwner() == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Gun %s has no owner"), *GetActorNameOrLabel());
			return;
		}
		
		if (PlayerCharacter == nullptr)
		{
			PlayerCharacter = Cast<AShooterCharacter>(GetOwner());
		}

		if (PlayerCharacter == nullptr)
		{
			return;
		}

		Rifle = Cast<ARifle>(this);
		Launcher = Cast<ALauncher>(this);

		PlayerCharacter->StopSprint();

		if (Rifle)
		{
			Rifle->PullTrigger(PlayerCharacter);
		}
		else if (Launcher)
		{
			Launcher->PullTrigger(PlayerCharacter);
		}

		Ammo--;
		if (Ammo == 0)
		{
			StartReload();
		}
		// UE_LOG(LogTemp, Warning, TEXT("%s Ammo: %i"), *GetActorNameOrLabel(), Ammo);	
		
	}
}

USkeletalMeshComponent* ABaseGun::GetMesh() const
{
	return Mesh;
}

USceneComponent* ABaseGun::GetMuzzle() const
{
	return Muzzle;
}

// int ABaseGun::GetAmmo() const
// {
// 	return Ammo;
// }

bool ABaseGun::CanShoot()
{
	if (Ammo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ammo!"));
		Rifle = Cast<ARifle>(this);
		if (Rifle != nullptr)
		{
			Rifle->HideMuzzleFlash();
		}
		return false;
	}
	else if (!bIsReloading)
	{
		return true;
	}

	if (Rifle != nullptr)
	{
		Rifle->HideMuzzleFlash();
	}
	return false;

}


bool ABaseGun::GunTrace(TArray<FHitResult> &HitActorArray, FVector Start, FVector End)
{
	// FCollisionQueryParams Params;
	// Params.AddIgnoredActor(this);
	// Params.AddIgnoredActor(PlayerCharacter);
	// Params.AddIgnoredComponent(PlayerCharacter->GetCapsuleComponent());
	// Params.AddIgnoredComponent(Mesh);
	// GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(PlayerCharacter);
	UKismetSystemLibrary::LineTraceMulti(
		this,
		Start,
		End,
		ETraceTypeQuery::TraceTypeQuery3,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitActorArray,
		true
	);

	if (HitActorArray.Num() > 0)
	{
		return true;
	}

	return false;
	// return OutHit.IsValidBlockingHit();
}

void ABaseGun::StartShoot()
{	
	if (IsFullyAuto)
	{
		Shoot();
		GetWorldTimerManager().SetTimer(ShootHandle, ShootDelegate, FireRate, true);
	}
	else
	{
		Shoot();
	}
}

void ABaseGun::StopShoot()
{
	GetWorldTimerManager().ClearTimer(ShootHandle);
	Rifle = Cast<ARifle>(this);
	if (Rifle == nullptr)
	{
		return;
	}
	Rifle->HideMuzzleFlash();
}

bool ABaseGun::CanReload()
{
	if (Ammo >= MagSize)
	{
		UE_LOG(LogTemp, Warning, TEXT("Mag is already full!"));
		return false;
	}
	if (ReserveAmmo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No reserve ammo!"));
		return false;
	}
	if (ReloadHandle.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Already reloading!"));
		return false;
	}
	return true;
}
void ABaseGun::StartReload()
{
	if (CanReload())
	{
		PlayerCharacter->StopSprint();
		bIsReloading = true;
		GetWorldTimerManager().SetTimer(ReloadHandle, ReloadDelegate, ReloadTime, false);
		UE_LOG(LogTemp, Warning, TEXT("Reloading!"));
	}
}

void ABaseGun::CancelReload()
{
	if (ReloadHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(ReloadHandle);
		bIsReloading = false;
		UE_LOG(LogTemp, Warning, TEXT("Reload cancelled!"));
	}
}

void ABaseGun::Reload()
{
	int MissingAmmo = MagSize - Ammo;

	if (ReserveAmmo >= MissingAmmo)
	{
		Ammo = Ammo + MissingAmmo;
		ReserveAmmo = ReserveAmmo - MissingAmmo;
	}
	else
	{
		Ammo = Ammo + ReserveAmmo;
		ReserveAmmo = 0;
	}
	
	GetWorldTimerManager().ClearTimer(ReloadHandle);
	bIsReloading = false;
	if (ReloadSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ReloadSound);
	}
	UE_LOG(LogTemp, Warning, TEXT("Reloaded!"));
}

void ABaseGun::OnMaxAmmo()
{
	Ammo = MagSize;
	ReserveAmmo = MaxReserveAmmo;
}

float ABaseGun::ReloadProgress() const
{
	return GetWorldTimerManager().GetTimerRemaining(ReloadHandle) / ReloadTime;
}

void ABaseGun::ShowHitmarker()
{
	bMakeHitmarkerVisible = true;
}

void ABaseGun::HideHitmarker()
{
	bMakeHitmarkerVisible = false;
}