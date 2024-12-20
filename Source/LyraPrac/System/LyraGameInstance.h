// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LyraGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LYRAPRAC_API ULyraGameInstance : public UGameInstance
{
	GENERATED_BODY()

	/**
	 * UGameInstance's interfaces
	 */
	virtual void Init() override;
	virtual void Shutdown() override;

};
