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
		if (ActivePlayMode == EPlayMode::MODE_Default)
		{
			TogglePlayMode();
		}
		else
		{
			OnAbilityOneExit();
		}
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
	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_GameTraceChannel1, Params);

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
		// UE_LOG(LogTemp, Warning, TEXT("Spawn portal called"));
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
		ActivePlayMode = EPlayMode::MODE_Default;

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
	UE_LOG(LogTemp, Warning, TEXT("Ability 2"));
}

void AShooterCharacter::OnAbilityUlt()
{
	UE_LOG(LogTemp, Warning, TEXT("Ability Ult"));
}