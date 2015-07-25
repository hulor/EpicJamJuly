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

	FVector _lastDir;

protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	void Fire();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Death)
		class UClass*	DeathFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fire)
		class UClass*	RocketActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fire)
		class UChildActorComponent* FireMusle;

public:
	ASamuraiRocketCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BlueprintCallable, Category = Death)
		void	Die();
};
