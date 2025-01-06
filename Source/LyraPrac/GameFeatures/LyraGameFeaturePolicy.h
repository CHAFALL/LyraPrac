#pragma once

#include "Containers/Array.h"
#include "GameFeatureStateChangeObserver.h"
#include "GameFeaturesProjectPolicies.h"
#include "UObject/Object.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "LyraGameFeaturePolicy.generated.h"

// GameFeature를 계속 사용하면 굉장히 많은 규칙들이 필요로 할텐데 
// 그거를 GameFeature 서브 시스템을 상속 받아서 관리하는 것은 굉장히 복잡
// -> 그래서 UDefaultGameFeaturesProjectPolicies를 상속 받아서 써라 (UDefaultGameFeaturesProjectPolicies는 UGameFeaturesProjectPolicies를 한번 더 랩핑 시켜서 편하게 만든 것임.)
// 즉, GameFeature 서브 시스템에서 우리가 원하는 규칙을 만들고 싶으면 UDefaultGameFeaturesProjectPolicies를 상속 받으면 됨.

/**
 * ULyraGameplayFeaturePolicy는 GameFeature Plugin이 Memory에 로딩되거나 Active(활성화)를 관리하는 StateMachine을 모니터링 가능하다
 */
UCLASS()
class ULyraGameplayFeaturePolicy : public UDefaultGameFeaturesProjectPolicies
{
	GENERATED_BODY()
public:
	ULyraGameplayFeaturePolicy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	* GameFeaturesProjectPolicies interfaces
	*/
	virtual void InitGameFeatureManager() override;
	virtual void ShutdownGameFeatureManager() override;

	/** Observer로서 등록한 객체를 관리 (아래 LyraGameFeature_AddGameplayCuePaths가 하나의 예시) */
	UPROPERTY(Transient)
	TArray<TObjectPtr<UObject>> Observers;

	// Observer 패턴이란?
	// Observer 패턴은 객체 간의 일대다(one-to-many) 의존 관계를 정의하는 행위 디자인 패턴입니다. 
	// 한 객체(Subject 또는 Observable)의 상태가 변경되면, 이 객체에 의존하는 모든 객체들(Observer들)이 자동으로 통지를 받고 갱신되는 방식입니다.

	// 여기서는 IGameFeatureStateChangeObserver를 통해 GameFeature 서브 시스템에 등록을 해두면 변화가 있을 때마다 알려줌. (그 중 OnGameFeatureRegistering, OnGameFeatureUnregistering 이 두 경우일 때만 알려줌)

	// 그럼 누군가는 Observer를 등록시켜줘야 됨. -> 그걸 ULyraGameplayFeaturePolicy가 해줌.
};

/**
 * GameFeature Plugin이 Register/Unregister 단계에서 GameplayCuePath가 등록되어 GameplayCueManager가 관련 GameplayCue 에셋을 스캔할 수 있도록 해야 한다:
 * 이를 위해 GameFeatureStateChangeObserver를 활용하여, 옵저버 패턴으로 이를 가능하게 하도록 한다:
 * - 아래에 우리가 Override할 메서드에서 유추할 수 있다시피 GameFeatureStateChangeObserver를 활용하면 가능하다!
 * - 이를 위해 추가적 구현이 필요한데 이는 ULyraGameplayFeaturePolicy를 참고하자.
 *
 * 참고로, GameFeatureAction에도 Registering/Unregistering이 있긴하다:
 * - 허나, 우리가 앞서 구현했다시피, 직접 Registering/Unregistering 호출해줘야 한다
 *   - Registering/Unregistering 구현해봐야 호출하지 않으면 아무 의미 없다...
 * - Lyra에서는 GameplayFeaturePolicy를 오버라이딩하는 방식으로 이를 진행하였다
 */
UCLASS()
class ULyraGameFeature_AddGameplayCuePaths : public UObject, public IGameFeatureStateChangeObserver
{
	GENERATED_BODY()
public:
	/**
	 * IGameFeatureStateChangeObserver interface
	 */
	virtual void OnGameFeatureRegistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL) override;
	virtual void OnGameFeatureUnregistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL) override;
};

