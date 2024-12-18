// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraGameModeBase.h"
#include "LyraExperienceManagerComponent.h"
#include "LyraGameState.h"
#include "LyraPrac/LyraLogChannels.h"
#include "LyraPrac/Character/LyraCharacter.h"
#include "LyraPrac/Player/LyraPlayerController.h"
#include "LyraPrac/Player/LyraPlayerState.h"

ALyraGameModeBase::ALyraGameModeBase()
{
	// LyraGameMode에서 아무런 설정을 하지 않았다면 얘네들로 기본 설정됨.
	GameStateClass = ALyraGameState::StaticClass();
	PlayerControllerClass = ALyraPlayerController::StaticClass();
	PlayerStateClass = ALyraPlayerState::StaticClass();
	DefaultPawnClass = ALyraCharacter::StaticClass();
}

void ALyraGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// 아직 GameInstance를 통해, 초기화 작업이 진행되므로, 현 프레임에는 Lyra의 Concept인 Experience 처리를 진행할 수 없다:
	// - 이를 처리하기 위해, 한프레임 뒤에 이벤트를 받아 처리를 이어서 진행한다
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);

}

void ALyraGameModeBase::InitGameState()
{
	Super::InitGameState();

	// Experience 비동기 로딩을 위한 Delegate를 준비한다:
	ULyraExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	// OnExperienceLoaded 등록
	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnLyraExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void ALyraGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// 로딩이 되어야지만 스폰.
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

APawn* ALyraGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	// 이 로그가 찍힌다는 것은 폰이 스폰이 되었다는 것임.
	UE_LOG(LogLyra, Log, TEXT("SpawnDefaultPawnAtTransform_Implementation is called!"));
	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
}

// 우리가 experience를 결정해주면 해당하는 experience를 로딩을 시작해줄 것임.
// Experience를 로딩하기 위해 Experience 매니저에게 요청하는 함수지만,
// 데디케이티드 서버 중심으로 개발되어 여러 조건을 체크해야 해서 
// HandleMatchAssignmentIfNotExpectingOne이라는 이름을 가짐.
void ALyraGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
}

bool ALyraGameModeBase::IsExperienceLoaded() const
{
	check(GameState);
	ULyraExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	return ExperienceManagerComponent->IsExperienceLoaded();
}

void ALyraGameModeBase::OnExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience)
{
}
