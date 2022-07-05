// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "DrawDebugHelpers.h"
#include "BaseGun.h"
#include "Rifle.h"
#include "Launcher.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilityPortal.h"
#include "Health.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterPlayerController.h"



AShooterCharacter::AShooterCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	HealthComp = CreateDefaultSubobject<UHealth>("Health Component");

	GetCharacterMovement()->MaxWalkSpeed = StrafeSpeed;

	ActivePlayMode = EPlayMode::MODE_Default;

}

void AShooterCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (PrimaryGunClass)
	{
		PrimaryGun = GetWorld()->SpawnActor<ARifle>(PrimaryGunClass);
		PrimaryGun->AttachToComponent(Mesh1P, FAttachmentTransformRules::KeepRelativeTransform, TEXT("GripPoint"));
		PrimaryGun->GetMesh()->SetVisibility(true);
		PrimaryGun->SetOwner(this);

	}

	if (SecondaryGunClass)
	{
		SecondaryGun = GetWorld()->SpawnActor<ALauncher>(SecondaryGunClass);
		SecondaryGun->AttachToComponent(Mesh1P, FAttachmentTransformRules::KeepRelativeTransform, TEXT("GripPoint"));
		SecondaryGun->GetMesh()->SetVisibility(false);
		SecondaryGun->SetOwner(this);

	}
	
	if (PrimaryGun)
	{
		ActiveGun = PrimaryGun;
	}

	if (SecondaryGun)
	{
		InactiveGun = SecondaryGun;
	}

	RifleGun = Cast<ARifle>(PrimaryGun);
	LauncherGun = Cast<ALauncher>(SecondaryGun);
	ShooterController = Cast<AShooterPlayerController>(GetWorld()->GetFirstPlayerController());

	AbilityTwoTickDelegate = FTimerDelegate::CreateUObject(this, &AShooterCharacter::AbilityTwoTick);
	AbilityUltDurationDelegate = FTimerDelegate::CreateUObject(this, &AShooterCharacter::EndAbilityUlt);

	// Bind select weapon bindings through delegate
	DECLARE_DELEGATE_OneParam(FSelectActiveGunDelegate, ABaseGun*);
	InputComponent->BindAction<FSelectActiveGunDelegate>("SelectWeapon1", IE_Pressed, this, &AShooterCharacter::SelectActiveGun, PrimaryGun);
	InputComponent->BindAction<FSelectActiveGunDelegate>("SelectWeapon2", IE_Pressed, this, &AShooterCharacter::SelectActiveGun, SecondaryGun);
}

void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// UE_LOG(LogTemp, Warning, TEXT("current cd: %f"), AbilityOneCurrentCooldown);

	if (!bAbilityOneReady)
	{
		AbilityOneCurrentCooldown = AbilityOneCurrentCooldown - DeltaTime;

		if (AbilityOneCurrentCooldown <= 0)
		{
			AbilityOneCurrentCooldown = 0;
			bAbilityOneReady = true;
		}
	}

	if (!bAbilityTwoReady)
	{
		AbilityTwoCurrentCooldown = AbilityTwoCurrentCooldown - DeltaTime;

		if (AbilityTwoCurrentCooldown <= 0)
		{
			AbilityTwoCurrentCooldown = 0;
			bAbilityTwoReady = true;
		}
	}

	if (!bAbilityUltReady)
	{
		AbilityUltCurrentCooldown = AbilityUltCurrentCooldown - DeltaTime;

		if (AbilityUltCurrentCooldown <= 0)
		{
			AbilityUltCurrentCooldown = 0;
			bAbilityUltReady = true;
		}
	}
}

bool AShooterCharacter::IsSameGun(ABaseGun* SwapTargetGun)
{
	if (SwapTargetGun == ActiveGun)
	{
		return true;
	}
	return false;
}

void AShooterCharacter::SelectActiveGun(ABaseGun* newActiveGun)
{
	if (IsSameGun(newActiveGun))
	{
		return;
	}
	
	ActiveGun->GetMesh()->SetVisibility(false);
	ActiveGun->CancelReload();
	InactiveGun = ActiveGun;

	ActiveGun = newActiveGun;
	ActiveGun->GetMesh()->SetVisibility(true);
	if (ActiveGun->GetAmmo() == 0)
	{
		ActiveGun->StartReload();
	}
	
	OnAbilityOneExit();
}

void AShooterCharacter::ToggleActiveGun()
{
	ActiveGun->StopShoot();

	if (ActiveGun == PrimaryGun)
	{
		SelectActiveGun(SecondaryGun);
	}
	else
	{
		SelectActiveGun(PrimaryGun);
	}
}

void AShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AShooterCharacter::OnShootPressed);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AShooterCharacter::OnShootReleased);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AShooterCharacter::OnReload);

	// Bind weapon swapping
	PlayerInputComponent->BindAction("ToggleWeapon", IE_Pressed, this, &AShooterCharacter::ToggleActiveGun);
	// SelectWeapon bindings bound in BeginPlay()

	//Bind combat abilities
	PlayerInputComponent->BindAction("Ability1", IE_Pressed, this, &AShooterCharacter::OnAbilityOne);
	PlayerInputComponent->BindAction("Ability2", IE_Pressed, this, &AShooterCharacter::OnAbilityTwo);
	PlayerInputComponent->BindAction("AbilityUlt", IE_Pressed, this, &AShooterCharacter::OnAbilityUlt);
	PlayerInputComponent->BindAction("CancelAbility", IE_Pressed, this, &AShooterCharacter::OnAbilityOneExit);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AShooterCharacter::OnInteract);
	PlayerInputComponent->BindAction("TogglePause", IE_Pressed, this, &AShooterCharacter::OnTogglePause).bExecuteWhenPaused = true;

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AShooterCharacter::OnSprint);

	PlayerInputComponent->BindAxis("LookRight", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AShooterCharacter::OnReload()
{
	if (ActiveGun)
	{
		ActiveGun->StartReload();
	}
}

void AShooterCharacter::OnShootPressed()
{
	if (ActiveGun == nullptr || bIsShootPressed)
	{
		return;
	}

	if (ActivePlayMode == EPlayMode::MODE_Default)
	{
		bIsShootPressed = true;
		ActiveGun->StartShoot();
	}

	if (ActivePlayMode == EPlayMode::MODE_Ability)
	{
		if (bAbilityOneReady)
		{
			HandlePortalSpawn();
		}
	}
}

void AShooterCharacter::OnShootReleased()
{
	bIsShootPressed = false;
	ActiveGun->StopShoot();
}

void AShooterCharacter::TogglePlayMode()
{
	if (ActivePlayMode == EPlayMode::MODE_Default)
	{
		ActiveGun->StopShoot();
		ActivePlayMode = EPlayMode::MODE_Ability;
		UE_LOG(LogTemp, Warning, TEXT("Ability mode"));
	}
	else
	{
		ActivePlayMode = EPlayMode::MODE_Default;
		UE_LOG(LogTemp, Warning, TEXT("Default mode"));
	}
}

void AShooterCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
	if (Value <= 0 && bIsSprinting)
	{
		StopSprint();
	}
}

void AShooterCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AShooterCharacter::OnSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	ActiveGun->CancelReload();
	bIsSprinting = true;
	UE_LOG(LogTemp, Warning, TEXT("Start sprint"));
}

void AShooterCharacter::StopSprint()
{
	if (!bIsSprinting)
	{
		return;
	}
	GetCharacterMovement()->MaxWalkSpeed = StrafeSpeed;
	bIsSprinting = false;
	UE_LOG(LogTemp, Warning, TEXT("Stop sprint"));
}

void AShooterCharacter::OnAbilityOne()
{
	if (bAbilityOneReady)
	{
		HandlePortalSpawn();
		
		// if (ActivePlayMode == EPlayMode::MODE_Default)
		// {
		// 	TogglePlayMode();
		// }
		// else
		// {
		// 	OnAbilityOneExit();
		// }
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("On cooldown: %f"), AbilityOneCurrentCooldown);
	}
	
}

void AShooterCharacter::OnAbilityOneExit()
{
	if (ActivePlayMode == EPlayMode::MODE_Ability)
	{
		if (FirstPortal)
		{
			FirstPortal->Destroy();
		}
		if (SecondPortal)
		{
			SecondPortal->Destroy();
		}
		ClearPortalsRef();
		TogglePlayMode();
	}
}

AAbilityPortal* AShooterCharacter::TracePortal(bool& bSuccess)
{
	FHitResult OutHit;
	AAbilityPortal* SpawnedPortal = nullptr;
	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector End = Start + (FirstPersonCameraComponent->GetForwardVector() * AbilityRange);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(PrimaryGun);
	Params.AddIgnoredActor(SecondaryGun);
	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_GameTraceChannel2, Params);

	if (AbilityPortalClass == nullptr)
	{
		bSuccess = false;
		return nullptr;
	}

	if (OutHit.IsValidBlockingHit())
	{
		DrawDebugSphere(GetWorld(), OutHit.ImpactPoint, 24, 8, FColor::Purple, false, 5);
		
		SpawnedPortal = GetWorld()->SpawnActor<AAbilityPortal>
		(
			AbilityPortalClass,
			OutHit.ImpactPoint,
			FRotator(0, 0, 0)
		);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Target not in range"));
		bSuccess = false;
		return nullptr;
	}
	
	if (SpawnedPortal == nullptr)
	{
		bSuccess = false;
		return nullptr;
	}
	bSuccess = true;
	return SpawnedPortal;
}

void AShooterCharacter::HandlePortalSpawn()
{
	bool bSuccess;
	AAbilityPortal* Portal = TracePortal(bSuccess);

	if (!bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't spawn portal"));
		return;
	}

	if (SecondPortal != nullptr)
	{
		ClearPortalsRef();
	}
	if (FirstPortal == nullptr)
	{
		// spawning first portal
		FirstPortal = Portal;
	}
	else
	{
		// spawning second portal
		SecondPortal = Portal;

		FirstPortal->SetupPortal(SecondPortal);
		SecondPortal->SetupPortal(FirstPortal);

		AbilityOneCurrentCooldown = AbilityOneCooldown;
		bAbilityOneReady = false;
		// ActivePlayMode = EPlayMode::MODE_Default;

		GetWorldTimerManager().SetTimer(AbilityOneCooldownHandle, AbilityOneCooldown, false);

	}
}

void AShooterCharacter::ClearPortalsRef()
{
	FirstPortal = nullptr;
	SecondPortal = nullptr;
}

void AShooterCharacter::OnAbilityTwo()
{
	if (bAbilityTwoReady)
	{
		bAbilityTwoReady = false;
		bAbilityTwoActive = true;
		AbilityTwoCurrentCooldown = AbilityTwoCooldown;

		CurrentTick = 0;

		GetWorldTimerManager().SetTimer(AbilityTwoTickHandle, AbilityTwoTickDelegate, 2, true);
		UE_LOG(LogTemp, Warning, TEXT("Ability Two"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("On cooldown: %f"), AbilityTwoCurrentCooldown);
	}
}

void AShooterCharacter::AbilityTwoTick()
{
	CurrentTick++;

	if (bIsAlive)
	{
		HealthComp->SetHealth(HealthComp->GetHealth() + HealthPerTick);
		if (HealthComp->GetHealth() >= HealthComp->GetMaxHealth())
		{
			HealthComp->SetHealth(HealthComp->GetMaxHealth());
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), HealthComp->GetHealth());

	if (CurrentTick >= TickLimit)
	{
		GetWorldTimerManager().ClearTimer(AbilityTwoTickHandle);
		bAbilityTwoActive = false;
		UE_LOG(LogTemp, Warning, TEXT("Ability two ended"));
	}
}

void AShooterCharacter::OnAbilityUlt()
{
	if (bAbilityUltReady)
	{
		bAbilityUltReady = false;

		bIsPlayerVisible = false;
		// add move speed

		GetWorldTimerManager().SetTimer(AbilityUltDurationHandle, AbilityUltDurationDelegate, AbilityUltDuration, false);
	
		AbilityUltCurrentCooldown = AbilityUltCooldown;

		ShooterController->ShowAbilityUlt();

		UE_LOG(LogTemp, Warning, TEXT("Ability Ult"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("On cooldown: %f"), AbilityUltCurrentCooldown);
	}

}

void AShooterCharacter::EndAbilityUlt()
{
	bIsPlayerVisible = true;
	ShooterController->HideAbilityUlt();
	// return move speed to normal
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	AProjectile* Projectile = Cast<AProjectile>(DamageCauser);
	if (Projectile)
	{
		DamageToApply = DamageToApply / 4;
	}

	DamageToApply = FMath::Min(HealthComp->GetHealth(), DamageToApply);
	DamageToApply = FMath::RoundHalfFromZero(DamageToApply);
	HealthComp->SetHealth(HealthComp->GetHealth() - DamageToApply);

	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), HealthComp->GetHealth());

	if (HealthComp->GetHealth() <= 0)
	{
		HandleDeath();
	}

	return DamageToApply;
}

void AShooterCharacter::HandleDeath()
{
	if (bIsAlive == true)
	{
		bIsAlive = false;
	
		DisableInput(GetWorld()->GetFirstPlayerController());
		OnShootReleased();
		if (DeathSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), DeathSound);
		}
		UE_LOG(LogTemp, Warning, TEXT("Player is dead"));
	}
}

void AShooterCharacter::OnMaxAmmo()
{
	ActiveGun->OnMaxAmmo();
	InactiveGun->OnMaxAmmo();
	ActiveGun->CancelReload();
}

bool AShooterCharacter::OnHealthUp(float HealthToAdd)
{	
	if (HealthComp->GetHealth() == HealthComp->GetMaxHealth())
	{
		return false;
	}
	
	HealthComp->SetHealth(HealthComp->GetHealth() + HealthToAdd);

	if (HealthComp->GetHealth() > HealthComp->GetMaxHealth())
	{
		HealthComp->SetHealth(HealthComp->GetMaxHealth());
	}
	return true;
}

void AShooterCharacter::OnInteract()
{
	InteractTrace();
}

void AShooterCharacter::OnTogglePause()
{
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		ShooterController->HidePauseMenu();
		ShooterController->bShowMouseCursor = false;
		ShooterController->SetInputMode(FInputModeGameOnly());
		return;
	}
	
	ShooterController->ShowPauseMenu();
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	ShooterController->bShowMouseCursor = true;
	ShooterController->SetInputMode(FInputModeGameAndUI());
}