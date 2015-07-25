// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SamuraiRocket.h"
#include "SamuraiRocketGameMode.h"
#include "SamuraiRocketCharacter.h"

ASamuraiRocketGameMode::ASamuraiRocketGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScroller/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
