// Fill out your copyright notice in the Description page of Project Settings.

#include "SamuraiRocket.h"
#include "CustomBPLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerState.h"


void	UCustomBPLibrary::SetPlayerStateScore(APlayerState* ps, int32 score)
{
	if (ps == NULL)
		return;
	ps->Score = score;
}

