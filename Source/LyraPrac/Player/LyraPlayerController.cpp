// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraPlayerController.h"
#include "LyraPrac/Camera/LyraPlayerCameraManager.h"

ALyraPlayerController::ALyraPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerCameraManagerClass = ALyraPlayerCameraManager::StaticClass();
}
