// Copyright Epic Games, Inc. All Rights Reserved.

#include "TargetPracticeCPPCharacter.h"
#include "TargetPracticeCPPProjectile.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"

//////////////////////////////////////////////////////////////////////////
// ATargetPracticeCPPCharacter

ATargetPracticeCPPCharacter::ATargetPracticeCPPCharacter()
{
	// Character starts without a rifle. I can change this from Blueprint if I want.
	bHasRifle = false;

	// Capsule size for this character
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.f);

	// I want the controller yaw to control the character (typical for a first person setup).
	bUseControllerRotationYaw = true;

	// ---------- CAMERA SETUP ----------

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// ---------- FIRST PERSON MESH (ARMS) ----------

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);

	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;

	// Tuned by trial and error so the arms look okay in first person.
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// ---------- DEFAULT MOVEMENT SETTINGS ----------

	MoveSpeed = 1.0f;
	LookSensitivity = 1.0f;
}

void ATargetPracticeCPPCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Register the default input mapping context. 
	// If this fails, I log a warning so I know something is wrong with input setup.
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				if (DefaultMappingContext)
				{
					Subsystem->AddMappingContext(DefaultMappingContext, 0);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("DefaultMappingContext is not set on %s"), *GetName());
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("EnhancedInputLocalPlayerSubsystem is null in %s"), *GetName());
			}
		}
	}
	else
	{
		// This can happen for example in some editor or PIE preview cases.
		UE_LOG(LogTemp, Warning, TEXT("ATargetPracticeCPPCharacter has no valid PlayerController on BeginPlay."));
	}
}

////////////////////////////////////////////////////////////////////////////
// Input

void ATargetPracticeCPPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Let the parent class bind anything it needs.
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// I keep null checks here so if I forget to set an InputAction in Blueprint,
		// the game will not crash, it will just not bind that action.

		// Jump
		if (JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		}

		// Move
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATargetPracticeCPPCharacter::Move);
		}

		// Look
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATargetPracticeCPPCharacter::Look);
		}
	}
}

void ATargetPracticeCPPCharacter::Move(const FInputActionValue& Value)
{
	// X = Right/Left, Y = Forward/Backward (2D input)
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller == nullptr)
	{
		return;
	}

	// Use the controller rotation so movement feels correct in first person.
	const FRotator ControlRotation = Controller->GetControlRotation();
	FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Forward / backward
	if (!FMath::IsNearlyZero(MovementVector.Y))
	{
		AddMovementInput(ForwardDirection, MovementVector.Y * MoveSpeed);
	}

	// Right / left
	if (!FMath::IsNearlyZero(MovementVector.X))
	{
		AddMovementInput(RightDirection, MovementVector.X * MoveSpeed);
	}
}

void ATargetPracticeCPPCharacter::Look(const FInputActionValue& Value)
{
	// X = Yaw, Y = Pitch
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller == nullptr)
	{
		return;
	}

	if (!FMath::IsNearlyZero(LookAxisVector.X))
	{
		AddControllerYawInput(LookAxisVector.X * LookSensitivity);
	}

	if (!FMath::IsNearlyZero(LookAxisVector.Y))
	{
		AddControllerPitchInput(LookAxisVector.Y * LookSensitivity);
	}
}

void ATargetPracticeCPPCharacter::SetHasRifle(bool bNewHasRifle)
{
	// Simple setter. Later this could also trigger an animation or UI update.
	bHasRifle = bNewHasRifle;
}

bool ATargetPracticeCPPCharacter::GetHasRifle()
{
	return bHasRifle;
}
