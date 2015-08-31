// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "MenuController.generated.h"

/**
 * 
 */
UCLASS()
class SAMURAIROCKET_API AMenuController : public APlayerController
{
	GENERATED_BODY()

private:
	void Validate();
	void Back();
	void MoveUp();
	void MoveDown();

	/// Network

	UFUNCTION(reliable, server, WithValidation)
		void ServerValidate();
	UFUNCTION(reliable, server, WithValidation)
		void ServerBack();
	UFUNCTION(reliable, server, WithValidation)
		void ServerMoveUp();
	UFUNCTION(reliable, server, WithValidation)
		void ServerMoveDown();

protected:
	virtual void SetupInputComponent() override;

public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnValidate();
	UFUNCTION(BlueprintImplementableEvent)
		void OnBack();
	UFUNCTION(BlueprintImplementableEvent)
		void OnMoveUp();
	UFUNCTION(BlueprintImplementableEvent)
		void OnMoveDown();
	
};
