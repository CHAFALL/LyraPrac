// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "LyraPlayerController.generated.h"

class ULyraAbilitySystemComponent;
class ALyraPlayerState;
/**
 * 
 */
UCLASS()
class LYRAPRAC_API ALyraPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()
public:
	ALyraPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	* PlayerController interface
	*/
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	/**
	 * member methods
	 */
	ALyraPlayerState* GetLyraPlayerState() const;
	ULyraAbilitySystemComponent* GetLyraAbilitySystemComponent() const;
};


// 왜 인풋에 이런 처리(PostProcessInput)가 있을까?
// -> 실질적 모든 인풋 처리는 다 플레이어 컨트롤러에서 한다고 보면 됨.