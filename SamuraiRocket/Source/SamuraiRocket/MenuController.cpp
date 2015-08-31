// Fill out your copyright notice in the Description page of Project Settings.

#include "SamuraiRocket.h"
#include "MenuController.h"

DEFINE_LOG_CATEGORY_STATIC(LogController, All, All);

void AMenuController::SetupInputComponent()
{
	InputComponent = ConstructObject<UInputComponent>(UInputComponent::StaticClass(), this, "MenuInputComponent");
	UE_LOG(LogController, Log, TEXT("Input ? %d"), ((InputComponent == NULL)? 1 : 0));
	if (InputComponent == NULL)
		return;
	InputComponent->BindAction("MenuValidate", IE_Pressed, this, &AMenuController::Validate);
	InputComponent->BindAction("MenuBack", IE_Pressed, this, &AMenuController::Back);
	InputComponent->BindAction("MenuUp", IE_Pressed, this, &AMenuController::MoveUp);
	InputComponent->BindAction("MenuDown", IE_Pressed, this, &AMenuController::MoveDown);
}

void AMenuController::Validate()
{
	if (Role < ROLE_Authority)
	{
		ServerValidate();
	}
	else
	{
		OnValidate();
	}
}

void AMenuController::Back()
{
	if (Role < ROLE_Authority)
	{
		ServerBack();
	}
	else
	{
		OnBack();
	}
}

void AMenuController::MoveUp()
{
	if (Role < ROLE_Authority)
	{
		ServerMoveUp();
	}
	else
	{
		OnMoveUp();
	}
}

void AMenuController::MoveDown()
{
	if (Role < ROLE_Authority)
	{
		ServerMoveDown();
	}
	else
	{
		OnMoveDown();
	}
}

/// Network implementation
void AMenuController::ServerValidate_Implementation()
{
	OnValidate();
}

bool AMenuController::ServerValidate_Validate()
{
	return (true);
}

void AMenuController::ServerBack_Implementation()
{
	OnBack();
}

bool AMenuController::ServerBack_Validate()
{
	return (true);
}

void AMenuController::ServerMoveUp_Implementation()
{
	OnMoveUp();
}

bool AMenuController::ServerMoveUp_Validate()
{
	return (true);
}

void AMenuController::ServerMoveDown_Implementation()
{
	OnMoveDown();
}

bool AMenuController::ServerMoveDown_Validate()
{
	return (true);
}


