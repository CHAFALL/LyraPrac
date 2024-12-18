// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LyraPlayerState.generated.h"

class ULyraPawnData;
class ULyraExperienceDefinition;

/**
 * 
 */
UCLASS()
class LYRAPRAC_API ALyraPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	/**
	 * AActor's interface
	 */
	virtual void PostInitializeComponents() final;

	/**
	 * member methods
	 */
	void OnExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience);

	UPROPERTY()
	TObjectPtr<const ULyraPawnData> PawnData;
};

// 의구심
// 분명히 experience 로딩이 완료되었으면 거기에 내 pawn 데이터가 있을테고 
// 그러면 experience manager 컴포넌트에서 다 가지고 있는거 아냐?
// 근데 왜 굳이 playerState에서 따로 캐싱을 해두지?
// 
// -> 지금은 상관없지만 나중에 playerState에서 게임 어빌리티 시스템 컴포넌트를 만들어서 부착을 해줄 것임
// 그래서 어빌리티 시스템 컴포넌트에서 이 pawn 데이터를 참조해서 능력을 부여할 것이므로 미리 캐싱을 해놓는 것임

