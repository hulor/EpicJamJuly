// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SamuraiRocket.h"
#include "SamuraiRocketGameMode.h"
#include "SamuraiRocketCharacter.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerState.h"
#include "Runtime/Engine/Public/EngineUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogGM, All, All);

ASamuraiRocketGameMode::ASamuraiRocketGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScroller/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void	ASamuraiRocketGameMode::RespawnPlayer(AController* pc)
{
	if (pc == NULL || pc->PlayerState->Score <= -10)
	{
		int nbPlayer = 0;

		for (TActorIterator<ASamuraiRocketCharacter> it(GetWorld()); it; ++it)
		{
			++nbPlayer;
		}
		UE_LOG(LogGM, Log, TEXT("Player logged %i"), nbPlayer);
		if (pc->PlayerState->Score <= -10 &&
			nbPlayer <= 2)
		{
			GameEnded();
		}

		return;
	}
	APawn* pawn = this->SpawnDefaultPawnFor(pc, this->ChoosePlayerStart(pc));

	if (pawn == NULL)
		return;
	pc->Possess(pawn);
	pc->PlayerState->Score = pc->PlayerState->Score - 1;
}

void	ASamuraiRocketGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogGM, Log, TEXT("Player logged %s"), *NewPlayer->GetName());
}

void	ASamuraiRocketGameMode::GameEnded_Implementation()
{
	UE_LOG(LogGM, Log, TEXT("Game over."));

}

FVector	ASamuraiRocketGameMode::FindRespawnPoint(AController* pc)
{
	return (this->ChoosePlayerStart(pc)->GetActorLocation());
}
