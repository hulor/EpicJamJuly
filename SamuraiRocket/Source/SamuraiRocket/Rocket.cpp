// Fill out your copyright notice in the Description page of Project Settings.

#include "SamuraiRocket.h"
#include "Rocket.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ARocket::ARocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>("Root");

	_ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectilMoveComponent");

	_Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	_Mesh->AttachParent = root;

	_BoxCollider = CreateDefaultSubobject<UBoxComponent>("BoxCollider");
	_BoxCollider->AttachParent = _Mesh;

	InitialSpeed = 200.0f;

	SetActorEnableCollision(true);
	_Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

// Called when the game starts or when spawned
void ARocket::BeginPlay()
{
	Super::BeginPlay();
	_Mesh->OnComponentHit.AddDynamic(this, &ARocket::OnHit);
}

// Called every frame
void ARocket::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void	ARocket::SetDirection(FVector dir)
{
	_ProjectileComponent->Velocity = dir * InitialSpeed;
}

void	ARocket::OnHit(AActor *SelfActor, UPrimitiveComponent *OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	InitialSpeed = 0.0f;
	this->Destroy();
}

