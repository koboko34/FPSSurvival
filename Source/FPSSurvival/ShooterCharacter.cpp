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
}

void AShooterCharacter::ToggleActiveGun()
{
	
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
	
	bIsShootPressed = true;
	ActiveGun->StartShoot();
}

void AShooterCharacter::OnShootReleased()
{
	bIsShootPressed = false;
	ActiveGun->StopShoot();
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
	UE_LOG(LogTemp, Warning, TEXT("Ability 1"));
}

void AShooterCharacter::OnAbilityTwo()
{
	UE_LOG(LogTemp, Warning, TEXT("Ability 2"));
}

void AShooterCharacter::OnAbilityUlt()
{
	UE_LOG(LogTemp, Warning, TEXT("Ability Ult"));
}