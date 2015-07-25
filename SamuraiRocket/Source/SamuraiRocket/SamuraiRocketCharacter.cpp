// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SamuraiRocket.h"
#include "SamuraiRocketCharacter.h"
#include "Rocket.h"
#include "Runtime/Engine/Classes/Components/ChildActorComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogMyCharacter, All, All);

ASamuraiRocketCharacter::ASamuraiRocketCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->RelativeRotation = FRotator(0.f,180.f,0.f);

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1440.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	FireMusle = CreateDefaultSubobject<UChildActorComponent>("Musle");

	FireMusle->AttachParent = this->GetMesh();
	_lastDir = FVector::ZeroVector;
	RocketActor = NULL;
	DeathFX = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASamuraiRocketCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ASamuraiRocketCharacter::Fire);
	InputComponent->BindAxis("MoveRight", this, &ASamuraiRocketCharacter::MoveRight);
}

void ASamuraiRocketCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f,-1.f,0.f), Value);

	if (Value == 0.0f)
		return;
	_lastDir = FVector(0.f, -1.f, 0.f) * Value;
}

void	ASamuraiRocketCharacter::Fire()
{
	if (this->RocketActor != NULL)
	{
		FVector pos = FireMusle->GetComponentLocation();
		FRotator rot = FireMusle->GetComponentRotation();
		ARocket* rocket = Cast<ARocket>(this->GetWorld()->SpawnActor(this->RocketActor, &pos, &rot));

		if (rocket == NULL)
		{
			return;
		}
		UE_LOG(LogMyCharacter, Log, TEXT("Spawn rocket"));
		//if (_lastDir == FVector::ZeroVector)
			rocket->SetDirection(this->GetMesh()->GetForwardVector());
		/*else
			rocket->SetDirection(_lastDir);*/
	}
}

void	ASamuraiRocketCharacter::Die()
{
	if (this->DeathFX != NULL)
	{
		FVector pos = this->GetActorLocation();
		FRotator rot = this->GetActorRotation();

		this->GetWorld()->SpawnActor(this->DeathFX, &pos, &rot);
	}
}
