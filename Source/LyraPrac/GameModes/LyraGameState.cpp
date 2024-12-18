// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraGameState.h"
#include "LyraPrac/GameModes/LyraExperienceManagerComponent.h"

ALyraGameState::ALyraGameState()
{
	ExperienceManagerComponent = CreateDefaultSubobject<ULyraExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}
