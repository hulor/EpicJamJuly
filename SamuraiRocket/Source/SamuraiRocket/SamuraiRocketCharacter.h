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

	class AActor*	_wall;
	bool _stun;
	bool _isDodging;
	bool _wallJumpDone;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dodge)
		float	DodgeDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stun)
		float	StunDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
		float	WallJumpVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
		float	WallJumpZVelocity;

public:
	ASamuraiRocketCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	void	EndDodge();

	UFUNCTION(BlueprintCallable, Category = Stun)
		void	UnStun();
	UFUNCTION(BlueprintCallable, Category = Stun)
		void	Stun();

	UFUNCTION(BlueprintCallable, Category = Death)
		void	Die();
	UFUNCTION(BlueprintCallable, Category = Collision)
		void	OnWallOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable, Category = Collision)
		void	OnWallOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** A way to know if character is currently dodging*/
	UFUNCTION(BlueprintCallable, Category = Dodge)
		bool	IsDodging() const;

	UFUNCTION(BlueprintCallable, Category = Jump)
		void	OwnJump();

};
