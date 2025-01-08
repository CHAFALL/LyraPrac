// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraExperienceManagerComponent.h"
#include "GameFeaturesSubsystemSettings.h"
#include "LyraExperienceDefinition.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeatureAction.h"
#include "LyraExperienceActionSet.h"
#include "LyraPrac/System/LyraAssetManager.h"

// 내가 물어보는 시점에 이미 experience 로딩이 완료가 되어있으면 불필요한 처리를 줄임.
void ULyraExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnLyraExperienceLoaded::FDelegate&& Delegate)
{
	// IsExperienceLoaded() 구현
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		/**
		 * 참고로, 여러분들은 Delegate 객체를 자세히 살펴보면, 내부적으로 필요한 변수들은 메모리 할당해놓는다:
		 * TArray<int> a = {1, 2, 3, 4};
		 * delegate_type delegate = [a](){
		 *	return a.Num();
		 * }
		 * a는 delegate_type 내부에 new로 할당되어 있다. 복사 비용을 낮추기 위해 Move를 통해 하는 것을 잊지 말자!
		 */
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}

void ULyraExperienceManagerComponent::ServerSetCurrentExperience(FPrimaryAssetId ExperienceId)
{
	ULyraAssetManager& AssetManager = ULyraAssetManager::Get();

	TSubclassOf<ULyraExperienceDefinition> AssetClass;
	{
		FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
		// 동기식으로 로딩
		AssetClass = Cast<UClass>(AssetPath.TryLoad());
	}

	// 왜 CDO를 가져오는 걸까? (GetDefault를 통해 CDO 방식으로 가져옴)
	// cf. 스태틱 클래스는 U클래스의 메타 데이터의 집합인거고 CDO는 클래스의 기본값을 모아놓은 인스턴스화된 오브젝트
	// CDO는 클래스의 기본값이라 얘를 함부로 다루면 안됨!!! 근데 왜 들고옴?, Primary Asset의 GetprimaryAssetId의 기본 조건이 CDO가 아니면 안됨..
	// 그리고 블루프린트로 만들어줬던 것(Experience Definition)의 이유는 타고 올라가려면 블루프린트여야 된다?
	const ULyraExperienceDefinition* Experience = GetDefault<ULyraExperienceDefinition>(AssetClass);
	check(Experience != nullptr);
	check(CurrentExperience == nullptr);
	{
		// 그리고 CDO로 CurrentExperience를 설정한다!
		// 어떤 의도로 이렇게 코드를 작성한지는 코드를 쭉 읽어보고(StartExperienceLoad까지 읽어보자) 다시 생각해보자:
		CurrentExperience = Experience;
	}

	StartExperienceLoad();
}

// Experience의 에셋을 로드
PRAGMA_DISABLE_OPTIMIZATION
void ULyraExperienceManagerComponent::StartExperienceLoad()
{
	check(CurrentExperience);
	check(LoadState == ELyraExperienceLoadState::Unloaded);

	LoadState = ELyraExperienceLoadState::Loading;

	ULyraAssetManager& AssetManager = ULyraAssetManager::Get();

	// 이미 앞서, ServerSetCurrentExperience에서 우리는 ExperienceId를 넘겨주었는데, 여기서 CDO를 활용하여, GetPrimaryAssetId를 로딩할 대상으로 넣는다!
	// - 왜 이렇게 하는걸까?
	// - GetPrimaryAssetId를 좀 더 자세히보자:
	// - GetPrimaryAssetId를 살펴봄으로써, 아래의 두가지를 알 수 있다:
	//   1. 우리는 B_LyraDefaultExperience를 BP로 만든 이유
	//   2. CDO를 가져와서, GetPrimaryAssetId를 호출한 이유

	// 우리는 앞서 이미 CDO로 로딩하여, CDO를 사용하지 않고 CDO를 사용하여 로딩할 에셋을 지정하여, BundleAssetList에 추가해준다!

	TSet<FPrimaryAssetId> BundleAssetList;
	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());

	// (추가)
	// ExperienceActionSet의 순회하며, BundleAssetList로 추가하자:
	for (const TObjectPtr<ULyraExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
	{
		if (ActionSet)
		{
			// 앞서, 우리가 생성한 HAS_Shooter_SharedHUD가 추가되겠다 (물론 추가적인 HAS_Shooter_XXX)도 추가될거다
			// - BundleAssetList는 Bundle로 등록할 Root의 PrimaryDataAsset를 추가하는 과정이다
			//   (->??? 무슨말인가 싶을건데 ChangeBundleStateForPrimaryAssets)을 살펴보면서 이해하자
			BundleAssetList.Add(ActionSet->GetPrimaryAssetId());
		}
	}


	// load assets associated with the experience
	// 아래는 우리가 후일 GameFeature를 사용하여, Experience에 바인딩된 GameFeature Plugin을 로딩할 Bundle 이름을 추가한다:
	// - Bundle이라는게 후일 우리가 로딩할 에셋의 카테고리 이름이라고 생각하면 된다 (일단 지금은 넘어가자 후일, 또 다룰 것이다!)
	TArray<FName> BundlesToLoad;
	{
		// 여기서 주목해야 할 부분은 OwnerNetMode가 NM_Standalone이면? Client/Server 둘다 로딩에 추가된다!
		const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
		bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
		bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);
		if (bLoadClient)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
		}
		if (bLoadServer)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
		}
	}

	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);

	// Good
	// 아래도, 후일 Bundle을 우리가 GameFeature에 연동하면서 더 깊게 알아보기로 하고, 지금은 앞서 B_LyraDefaultExperience를 로딩해주는 함수로 생각하자
	// 다양한 리스트 중에서 BundledLoad랑 동일하게 설정되어 있는 애들만 로딩할 수 있는 방식이 ChangeBundleStateForPrimaryAssets이다.
	// 의문점 : 클라이언트나, 서버 키워드가 있어야 로딩이 되어야 되는 것 아닌가? (번들에 관련된 것만 로딩을 하는 함수잖아?)
	// -> (Test) ULyraExperienceDefinition의 DefaultPawnData를 TObjectPtr가 아닌 TSoftObjectPtr로 정의한다면?
	// (Test Result) 아무것도 로딩이 안됨을 알 수 있음
	// 즉, TObjectPtr는 마이그레이션처럼 강하게 묶여있는 애들을 다 데리고 옴 (그래서 번들로 등록하지 않아도 같이 딸려오는 것임)
	// TSoftObjectPtr의 경우 걔의 위치를 알고 있는 것일 뿐이라 로딩을 한 뒤에 사용이 가능하다 그래서 다 끌고 오지 않음

	// 그래서 보면 클라랑 서버랑 상관없이 무조건 가져와야 되는 데이터들은 TObjectPtr로 설정이 되어있고
	// 일부만 알아야 되는 경우는 TSoftObjectPtr로 설정이 되어 있음을 알 수 있다.

	TSharedPtr<FStreamableHandle> Handle = AssetManager.ChangeBundleStateForPrimaryAssets(
		BundleAssetList.Array(),
		BundlesToLoad,
		{}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);

	if (!Handle.IsValid() || Handle->HasLoadCompleted())
	{
		// 로딩이 완료되었으면, ExecuteDelegate를 통해 OnAssetsLoadedDelegate를 호출하자:
		// - 아래의 함수를 확인해보자:
		FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
	}
	else
	{
		Handle->BindCompleteDelegate(OnAssetsLoadedDelegate);
		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]()
			{
				OnAssetsLoadedDelegate.ExecuteIfBound();
			}));
	}

	// FrameNumber를 주목해서 보자
	static int32 StartExperienceLoad_FrameNumber = GFrameNumber;
}
PRAGMA_ENABLE_OPTIMIZATION

// game feature를 로딩하고 활성화
void ULyraExperienceManagerComponent::OnExperienceLoadComplete()
{
	// FrameNumber를 주목해서 보자
	static int32 OnExperienceLoadComplete_FrameNumber = GFrameNumber;

	check(LoadState == ELyraExperienceLoadState::Loading);
	check(CurrentExperience);

	// 이전 활성화된 GameFeature Plugin의 URL을 클리어해준다
	GameFeaturePluginURLs.Reset();

	auto CollectGameFeaturePluginURLs = [This = this](const UPrimaryDataAsset* Context, const TArray<FString>& FeaturePluginList)
		{
			// FeaturePluginList를 순회하며, PluginURL을 ExperienceManagerComponent의 GameFeaturePluginURLS에 추가해준다
			for (const FString& PluginName : FeaturePluginList)
			{
				FString PluginURL;
				if (UGameFeaturesSubsystem::Get().GetPluginURLByName(PluginName, PluginURL))
				{
					This->GameFeaturePluginURLs.AddUnique(PluginURL);
				}
			}
		};

	// GameFeaturesToEnable에 있는 Plugin만 일단 활성화할 GameFeature Plugin 후보군으로 등록
	CollectGameFeaturePluginURLs(CurrentExperience, CurrentExperience->GameFeaturesToEnable);

	// GameFeaturePluginURLs에 등록된 Plugin을 로딩 및 활성화:
	// 활성화 해줘야 할 experience
	NumGameFeaturePluginsLoading = GameFeaturePluginURLs.Num();
	if (NumGameFeaturePluginsLoading)
	{
		LoadState = ELyraExperienceLoadState::LoadingGameFeatures;
		for (const FString& PluginURL : GameFeaturePluginURLs)
		{
			// 매 Plugin이 로딩 및 활성화 이후, OnGameFeaturePluginLoadComplete 콜백 함수 등록
			// 해당 함수를 살펴보도록 하자
			UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(PluginURL, FGameFeaturePluginLoadComplete::CreateUObject(this, &ThisClass::OnGameFeaturePluginLoadComplete));
		}
	}
	else
	{
		// 해당 함수가 불리는 것은 앞서 보았던 StreamableDelegateDelayHelper에 의해 불림
		OnExperienceFullLoadCompleted();
	}
}

void ULyraExperienceManagerComponent::OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result)
{
	// 매 GameFeature Plugin이 로딩될 때, 해당 함수가 콜백으로 불린다
	NumGameFeaturePluginsLoading--;
	if (NumGameFeaturePluginsLoading == 0)
	{
		// GameFeaturePlugin 로딩이 다 끝났을 경우, 기존대로 Loaded로서, OnExperienceFullLoadCompleted 호출한다
		// GameFeaturePlugin 로딩과 활성화가 끝났다면? UGameFeatureAction을 활성화해야겠지 (조금만 있다가 하장)
		OnExperienceFullLoadCompleted();
	}
}

PRAGMA_DISABLE_OPTIMIZATION
void ULyraExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
	check(LoadState != ELyraExperienceLoadState::Loaded);

	// GameFeature Plugin의 로딩과 활성화 이후, GameFeature Action들을 활성화 시키자:
	{
		LoadState = ELyraExperienceLoadState::ExecutingActions;

		// GameFeatureAction 활성화를 위한 Context 준비
		FGameFeatureActivatingContext Context;
		{
			// 월드의 핸들을 세팅해준다
			// WorldContext를 넘겨주는 이유?
			// -> UGameFeatureAction를 관리하는 UGameFeaturesSubsystem은 UEngineSubsystem를 상속받고 있음
			// 그렇다보니 엔진 서브시스템은 엔진이 종료되기 전까지 사라지지 않음...
			// 그래서 PIE를 아무리 만들고 소멸시켜도 이 PIE와 관련없이 엔진 서브시스템은 종료하기 전까지 영원히 남아있다는 뜻..
			// 데이터 참조 관련 문제와 어느 월드에다가 바인딩 처리를 해야되는지 모호
			// 이런 문제를 해결하기 위해서 World Context를 넘겨줌.
			// (참고) PIE -> 인스턴스가 독립적이라고 생각하지만(착각) 그냥 World로 구분되어있음
			const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
			if (ExistingWorldContext)
			{
				Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
			}
		}

		auto ActivateListOfActions = [&Context](const TArray<UGameFeatureAction*>& ActionList)
			{
				for (UGameFeatureAction* Action : ActionList)
				{
					// 명시적으로 GameFeatureAction에 대해 Registering -> Loading -> Activating 순으로 호출한다
					if (Action)
					{
						Action->OnGameFeatureRegistering();
						Action->OnGameFeatureLoading();
						Action->OnGameFeatureActivating(Context);
					}
				}
			};

		// 1. Experience의 Actions
		ActivateListOfActions(CurrentExperience->Actions);

		// 2. Experience의 ActionSets
		for (const TObjectPtr<ULyraExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
		{
			ActivateListOfActions(ActionSet->Actions);
		}
	}

	LoadState = ELyraExperienceLoadState::Loaded;
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();
}
PRAGMA_ENABLE_OPTIMIZATION

const ULyraExperienceDefinition* ULyraExperienceManagerComponent::GetCurrentExperienceChecked() const
{
	check(LoadState == ELyraExperienceLoadState::Loaded);
	check(CurrentExperience != nullptr);
	return CurrentExperience;
}
