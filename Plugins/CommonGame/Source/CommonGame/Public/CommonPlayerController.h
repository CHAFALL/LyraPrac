// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "CommonPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class COMMONGAME_API ACommonPlayerController : public AModularPlayerController
{
	GENERATED_BODY()
public:
	// 로컬 플레이어가 활성화되는 시점!!
	virtual void ReceivedPlayer() override;
};
