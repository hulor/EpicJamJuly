// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "SamuraiRocketCharacter.generated.h"

UCLASS(config=Game)
class ASamuraiRocketCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Trigger used to find wall */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* _WallTrigger;

	FVector _lastDir;

	bool _stun;
	bool _isDodging;

protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	void MoveUp(float Val);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	virtual void Landed(const FHitResult & Hit) override;

	void Fire();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Death)
		class UClass*	DeathFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fire)
		class UClass*	RocketActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fire)
		class UChildActorComponent* FireMusle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dodge)
		float	DodgeVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dodge)
		float	UpDodgeIntensity;

public:
	ASamuraiRocketCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BlueprintCallable, Category = Death)
		void	Die();
	UFUNCTION(BlueprintCallable, Category = Collision)
		void	OnWallOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** A way to know if character is currently dodging*/
	UFUNCTION(BlueprintCallable, Category = Dodge)
		bool	IsDodging() const;
};
