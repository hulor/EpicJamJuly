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
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 2000000.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	FireMusle = CreateDefaultSubobject<UChildActorComponent>("Musle");

	_WallTrigger = CreateDefaultSubobject<UBoxComponent>("WallTrigger");

	_WallTrigger->AttachParent = this->GetMesh();

	FireMusle->AttachParent = this->GetMesh();
	_lastDir = FVector::ZeroVector;
	RocketActor = NULL;
	DeathFX = NULL;
	DodgeVelocity = 200.0f;
	UpDodgeIntensity = 50.0f;
	_isDodging = false;
	_stun = false;
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
	InputComponent->BindAxis("MoveUp", this, &ASamuraiRocketCharacter::MoveUp);
}

void ASamuraiRocketCharacter::Landed(const FHitResult & Hit)
{
	Super::Landed(Hit);
	_isDodging = false;
}

void ASamuraiRocketCharacter::MoveRight(float Value)
{
	if (_stun == true)
		return;
	// add movement in that direction
	AddMovementInput(FVector(0.f,-1.f,0.f), Value);

	/*if (Value == 0.0f)
		return;*/
	_lastDir.Y = -Value;
}

void ASamuraiRocketCharacter::MoveUp(float Value)
{
	/*if (Value == 0.0f)
		return;*/
	_lastDir.Z = Value;
}


void	ASamuraiRocketCharacter::Fire()
{
	if (this->RocketActor != NULL &&
		_isDodging == false &&
		_stun == false)
	{
		FVector pos = FireMusle->GetComponentLocation();
		FRotator rot = FireMusle->GetComponentRotation();
		rot = FVector(_lastDir.Z, _lastDir.Y, 0.0f).Rotation();
		ARocket* rocket = Cast<ARocket>(this->GetWorld()->SpawnActor(this->RocketActor, &pos, &rot));

		if (rocket == NULL)
		{
			return;
		}
		FVector direction = _lastDir;

		if (direction == FVector::ZeroVector)
			direction = this->GetActorForwardVector();
		//UE_LOG(LogMyCharacter, Log, TEXT("Spawn rocket"));
		direction.Normalize();
		rocket->SetDirection(direction);

		float upIntensity = FMath::Abs(FVector::DotProduct(direction, FVector(0.f, 1.f, 0.f))) * UpDodgeIntensity;
		this->CharacterMovement->Launch(direction * DodgeVelocity + this->GetActorUpVector());
		_isDodging = true;
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

void	ASamuraiRocketCharacter::OnWallOverlapBegin(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (_isDodging == true)
		_stun = true;
}

bool	ASamuraiRocketCharacter::IsDodging() const
{
	return (_isDodging);
}
