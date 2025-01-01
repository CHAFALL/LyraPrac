// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "LyraPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LYRAPRAC_API ALyraPlayerController : public AModularPlayerController
{
	GENERATED_BODY()
public:
	ALyraPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
