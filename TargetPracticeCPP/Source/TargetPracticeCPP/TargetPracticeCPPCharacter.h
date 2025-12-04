// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "TargetPracticeCPPCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS(config = Game)
class ATargetPracticeCPPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATargetPracticeCPPCharacter();

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Setup input bindings for this character */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

private:
	// ---------- COMPONENTS ----------

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	// ---------- INPUT ----------

	/** Mapping context for enhanced input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	// ---------- MOVEMENT SETTINGS ----------

	/** Simple multiplier for movement speed, so I can tweak it from Blueprint */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 1.0f;

	/** Look sensitivity for yaw / pitch input */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float LookSensitivity = 1.0f;

	// ---------- WEAPON / STATE ----------

	/** Bool for AnimBP to switch to another animation set (rifle / no rifle) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	bool bHasRifle;

public:
	/** Setter to set the rifle state */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the rifle state */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool GetHasRifle();

	/** Returns Mesh1P subobject */
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	/** Returns FirstPersonCameraComponent subobject */
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
};
