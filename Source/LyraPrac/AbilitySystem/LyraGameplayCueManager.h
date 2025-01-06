// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueManager.h"
#include "LyraGameplayCueManager.generated.h"

/**
 * 
 */
UCLASS()
class LYRAPRAC_API ULyraGameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()
public:
	static ULyraGameplayCueManager* Get(); // 싱글톤

	ULyraGameplayCueManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void RefreshGameplayCuePrimaryAsset();
};
