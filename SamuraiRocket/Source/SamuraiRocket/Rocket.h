// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Rocket.generated.h"

UCLASS()
class SAMURAIROCKET_API ARocket : public AActor
{
	GENERATED_BODY()
protected:
	/** Rocket's projectil movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* _ProjectileComponent;

	/** Rocket's mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* _Mesh;

	/** Box collision of Rocket */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* _BoxCollider;

	FVector _direction;

public:
	/** Start speed of Rockets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float	InitialSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
		class UClass*	ExplosionFX;
	
public:	
	// Sets default values for this actor's properties
	ARocket();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void	SetDirection(FVector dir);

	UFUNCTION(BlueprintCallable, Category = Collision)
		void	OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
