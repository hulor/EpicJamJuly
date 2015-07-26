// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SamuraiRocket.h"
#include "SamuraiRocketCharacter.h"
#include "Rocket.h"
#include "SamuraiRocketGameMode.h"
#include "Runtime/Engine/Classes/Components/ChildActorComponent.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerState.h"
#include "UnrealNetwork.h"

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
	DodgeDuration = 1.5f;
	StunDuration = 1.0f;
	_isDodging = false;
	_stun = false;
	_hasWall = false;
	_wallActor = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Networking
void ASamuraiRocketCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASamuraiRocketCharacter, _wall);
	DOREPLIFETIME(ASamuraiRocketCharacter, _hasWall);
	DOREPLIFETIME(ASamuraiRocketCharacter, _isDodging);
	DOREPLIFETIME(ASamuraiRocketCharacter, _stun);
	DOREPLIFETIME(ASamuraiRocketCharacter, _wallJumpDone);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASamuraiRocketCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	InputComponent->BindAction("Jump", IE_Pressed, this, &ASamuraiRocketCharacter::OwnJump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ASamuraiRocketCharacter::Fire);
	InputComponent->BindAxis("MoveRight", this, &ASamuraiRocketCharacter::MoveRight);
	InputComponent->BindAxis("MoveUp", this, &ASamuraiRocketCharacter::MoveUp);
}

void ASamuraiRocketCharacter::Landed(const FHitResult & Hit)
{
	Super::Landed(Hit);
	_isDodging = false;
	_wallJumpDone = false;
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
	// not server
	if (Role < ROLE_Authority)
	{
		ServerFire(_lastDir);
	}
	else
	{
		RocketFire(_lastDir);
	}
}

void	ASamuraiRocketCharacter::RocketFire(FVector dir)
{
	if (this->RocketActor != NULL &&
		_isDodging == false &&
		_stun == false)
	{
		FVector direction = dir;

		if (direction == FVector::ZeroVector)
			direction = this->GetActorForwardVector();
		direction.Normalize();


		FVector pos = FireMusle->GetComponentLocation();
		FRotator rot = FireMusle->GetComponentRotation();
		float angle = FVector::DotProduct(direction, FVector::ForwardVector) * FVector::DotProduct(FVector::CrossProduct(direction, FVector::ForwardVector), FVector::RightVector);
		rot = FRotator::MakeFromEuler(FVector(FMath::RadiansToDegrees(angle), 0.f, 0.f));
		ARocket* rocket = Cast<ARocket>(this->GetWorld()->SpawnActor(this->RocketActor, &pos, &rot));

		if (rocket == NULL)
		{
			return;
		}
		//UE_LOG(LogMyCharacter, Log, TEXT("Spawn rocket"));
		rocket->SetDirection(direction);

		float upIntensity = FMath::Abs(FVector::DotProduct(direction, FVector(0.f, 1.f, 0.f))) * UpDodgeIntensity;
		direction = direction * DodgeVelocity + FVector::UpVector * upIntensity;
		this->GetCharacterMovement()->Launch(direction + this->GetActorUpVector());
		GetWorld()->GetTimerManager().SetTimer(this, &ASamuraiRocketCharacter::EndDodge, DodgeDuration, false);
		_isDodging = true;
		if (_hasWall == true)
		{
			Stun();
		}
	}
}

void	ASamuraiRocketCharacter::EndDodge()
{
	_isDodging = false;
}

void	ASamuraiRocketCharacter::UnStun()
{
	_stun = false;
}

void	ASamuraiRocketCharacter::Stun()
{
	_stun = true;
	_isDodging = false;
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	GetWorld()->GetTimerManager().SetTimer(this, &ASamuraiRocketCharacter::UnStun, StunDuration, false);
}

void	ASamuraiRocketCharacter::Die()
{
	if (this->DeathFX != NULL)
	{
		FVector pos = this->GetActorLocation();
		FRotator rot = this->GetActorRotation();

		this->GetWorld()->SpawnActor(this->DeathFX, &pos, &rot);
	}
	if (this->HasAuthority() == false)
		return;
	ASamuraiRocketGameMode* gm = Cast<ASamuraiRocketGameMode>(this->GetWorld()->GetAuthGameMode());

	if (gm == NULL)
		return;
	gm->RespawnPlayer(this->GetController());
	this->Destroy();
}

void	ASamuraiRocketCharacter::OnWallOverlapBegin(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (_isDodging == true &&
		this->HasAuthority() == true)
	{
		Stun();
	}
	if (OtherActor->ActorHasTag("Wall") == true ||
		OtherComp->ComponentHasTag("Wall") == true)
	{
		_wallActor = OtherActor;
		if (GetCharacterMovement()->IsFalling() == true)
		{
			FVector dir = (this->GetActorLocation() - OtherActor->GetActorLocation());
			this->SetActorRotation((FVector::DotProduct(dir, FVector::RightVector) * FVector::RightVector).Rotation());
		}
		if (this->HasAuthority() == true)
		{
			_wall = OtherActor->GetActorLocation();
			_hasWall = true;
			_wallJumpDone = false;
		}
	}
}

void	ASamuraiRocketCharacter::OnWallOverlapEnd(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == _wallActor)
	{
		_wallActor = NULL;
		_hasWall = false;
	}
}

bool	ASamuraiRocketCharacter::IsDodging() const
{
	return (_isDodging);
}

bool	ASamuraiRocketCharacter::IsStun() const
{
	return (_stun);
}

bool	ASamuraiRocketCharacter::HasWall() const
{
	return (_hasWall);
}

void	ASamuraiRocketCharacter::OwnJump()
{
	if (_stun == true)
		return;
	if (GetCharacterMovement()->IsFalling() == true &&
		_hasWall == true &&
		_wallJumpDone == false)
	{
		FVector jumpDir = FVector(0.f, 1.f, 0.f);
		FVector playerWall = (this->GetActorLocation() - _wall);

		playerWall.Normalize();
		jumpDir = jumpDir * FVector::DotProduct(playerWall, jumpDir) * WallJumpVelocity + FVector::UpVector * WallJumpZVelocity;

		GetCharacterMovement()->Launch(jumpDir);

		if (Role < ROLE_Authority)
		{
			ServerWallJump(jumpDir);
		}
		_wallJumpDone = true;
	}
	else
	{
		if (_hasWall == true)
		{
			FVector dir = (this->GetActorLocation() - _wall);
			this->SetActorRotation((FVector::DotProduct(dir, FVector::RightVector) * FVector::RightVector).Rotation());
		}
		Jump();
	}
}

////////////////////////////////////////////
// Network gameplay

void ASamuraiRocketCharacter::ServerFire_Implementation(FVector dir)
{
	_lastDir = dir;
	Fire();
}

bool ASamuraiRocketCharacter::ServerFire_Validate(FVector dir)
{
	return (true);
}


void ASamuraiRocketCharacter::ServerWallJump_Implementation(FVector dir)
{
	GetCharacterMovement()->Launch(dir);
}

bool ASamuraiRocketCharacter::ServerWallJump_Validate(FVector dir)
{
	return (true);
}
