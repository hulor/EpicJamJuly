// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "CustomBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SAMURAIROCKET_API UCustomBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = PlayerState)
		static void	SetPlayerStateScore(class APlayerState* ps, int32 score);
	
	
};
