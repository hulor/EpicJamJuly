// Fill out your copyright notice in the Description page of Project Settings.

#include "SamuraiRocket.h"
#include "Rocket.h"
#include "SamuraiRocketCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ARocket::ARocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>("Root");

	RootComponent = root;
	_ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectilMoveComponent");

	_Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	_Mesh->AttachParent = root;

	_BoxCollider = CreateDefaultSubobject<UBoxComponent>("BoxCollider");
	_BoxCollider->AttachParent = _Mesh;

	InitialSpeed = 200.0f;
	ExplosionFX = NULL;
}

// Called when the game starts or when spawned
void ARocket::BeginPlay()
{
	Super::BeginPlay();
	/*TScriptDelegate<FWeakObjectPtr> delegBegin;

	delegBegin.BindUFunction(this, FName("OnBeginOverlap"));
	_BoxCollider->OnComponentBeginOverlap.Add(delegBegin);*/
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

void	ARocket::OnOverlapBegin(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASamuraiRocketCharacter* target = Cast<ASamuraiRocketCharacter>(OtherActor);

	if (target != NULL)
	{
		target->Die();
	}
	if (this->ExplosionFX != NULL)
	{
		FVector pos = this->GetActorLocation();
		FRotator rot = this->GetActorRotation();

		this->GetWorld()->SpawnActor(this->ExplosionFX, &pos, &rot);
	}
	this->Destroy();
}

