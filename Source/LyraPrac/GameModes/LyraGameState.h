// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LyraGameState.generated.h"

class ULyraExperienceManagerComponent;

/**
 * 
 */
UCLASS()
class LYRAPRAC_API ALyraGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ALyraGameState();

public:
	UPROPERTY()
	TObjectPtr<ULyraExperienceManagerComponent> ExperienceManagerComponent;
};
