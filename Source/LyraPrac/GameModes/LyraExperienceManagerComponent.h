// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "GameFeaturePluginOperationResult.h"
#include "LyraExperienceManagerComponent.generated.h"

class ULyraExperienceDefinition;

// Experience 로딩 단계
enum class ELyraExperienceLoadState
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	ExecutingActions,
	Loaded,
	Deactivating,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLyraExperienceLoaded, const ULyraExperienceDefinition*);


/**
 * LyraExperienceManagerComponent
 * - 말 그대로, 해당 component는 game state를 owner로 가지면서, experience의 상태 정보를 가지고 있는 component이다
 * - 뿐만 아니라, manager라는 단어가 포함되어 있듯이, experience 로딩 상태 업데이트 및 이벤트를 관리한다
 */
UCLASS()
class LYRAPRAC_API ULyraExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()
	
public:

	// 외부에서 이 함수를 요청하면 로딩 여부를 파악 가능.
	/**
	 * member methods
	 */
	bool IsExperienceLoaded() { return (LoadState == ELyraExperienceLoadState::Loaded) && (CurrentExperience != nullptr); }

	/**
	 * 아래의 OnExperienceLoaded에 바인딩하거나, 이미 Experience 로딩이 완료되었다면 바로 호출함
	 */
	void CallOrRegister_OnExperienceLoaded(FOnLyraExperienceLoaded::FDelegate&& Delegate);

	void ServerSetCurrentExperience(FPrimaryAssetId ExperienceId);
	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	void OnExperienceFullLoadCompleted();
	const ULyraExperienceDefinition* GetCurrentExperienceChecked() const;


public:
	// 내가 어떤 것을 로딩할 것 인지.
	// 중요한 것은 얘가 이걸 들고 있다고 해서 로딩을 완료한 것은 아님. (장바구니 목록 느낌.)
	// 왜냐? 게임 모드 단계에서 이 정보를 가지고 매니저한테 로딩을 요청하는 것임
	// (cf. 실제 로딩은 Experience Manager가 담당)
	UPROPERTY()
	TObjectPtr<const ULyraExperienceDefinition> CurrentExperience;

	/** Experience의 로딩 상태를 모니터링 */
	ELyraExperienceLoadState LoadState = ELyraExperienceLoadState::Unloaded;

	/** Experience 로딩이 완료된 이후, Broadcasting Delegate */
	FOnLyraExperienceLoaded OnExperienceLoaded;

	/** 활성화된 GameFeature Plugin들 */
	int32 NumGameFeaturePluginsLoading = 0;
	TArray<FString> GameFeaturePluginURLs;
};
