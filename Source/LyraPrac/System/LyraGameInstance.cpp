// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraGameInstance.h"
#include "Components/GameFrameworkComponentManager.h"
#include "LyraPrac/LyraGameplayTags.h"



void ULyraGameInstance::Init()
{
	Super::Init();

	// InitState를 통해서 초기화 순서를 우리 방식대로 재정의 가능
	// 왜 이런 식으로 재정의? 컴포넌트 방식인 부품을 부착하는 방식을 이용할 것인데
	// 그 부품을 부착하는 방식 내에서 초기화 순서를 보장받을 수 없기 때문에 
	// 4가지 단계를 통해 관리

	// 앞서 정의한 InitState의 GameplayTags 등록:
	// UGameFrameworkComponentManager: 컴포넌트들의 초기화 상태(InitState)를 관리하고 추적하는 클래스
	// 이미 부착된 컴포넌트들의 초기화 상태와 순서를 관리하는 역할을 해줌. (ex. 무기 -> 스킬 -> 이펙트)
	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);
	if (ensure(ComponentManager))
	{
		const FLyraGameplayTags& GameplayTags = FLyraGameplayTags::Get();

		ComponentManager->RegisterInitState(GameplayTags.InitState_Spawned, false, FGameplayTag());
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataAvailable, false, GameplayTags.InitState_Spawned);
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataInitialized, false, GameplayTags.InitState_DataAvailable);
		ComponentManager->RegisterInitState(GameplayTags.InitState_GameplayReady, false, GameplayTags.InitState_DataInitialized);
	}
}

void ULyraGameInstance::Shutdown()
{
	Super::Shutdown();
}