// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "SamuraiRocketGameMode.generated.h"

UCLASS(minimalapi)
class ASamuraiRocketGameMode : public AGameMode
{
	GENERATED_BODY()

private:
	TMap<class AController*, USkeletalMesh*> _mapController;

public:
	ASamuraiRocketGameMode();

	void	RespawnPlayer(AController* pc);
	virtual void PostLogin(APlayerController * NewPlayer) override;

	UFUNCTION(BlueprintNativeEvent)
		void GameEnded();

	UFUNCTION(BlueprintCallable, Category = Respawn)
		FVector FindRespawnPoint(class AController* pc);

	UFUNCTION(BlueprintCallable, Category = Respawn)
		void	AddControllerSkeleton(class AController* pc, USkeletalMesh* skeleton);
};



