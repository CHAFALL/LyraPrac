#pragma once

#include "Containers/Map.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeatureAction_WorldActionBase.generated.h"

class FDelegateHandle;
class UGameInstance;
struct FGameFeatureActivatingContext;
struct FGameFeatureDeactivatingContext;
struct FWorldContext;

UCLASS(Abstract)
class UGameFeatureAction_WorldActionBase : public UGameFeatureAction
{
	GENERATED_BODY()
public:
	/**
	 * UGameFeatureAction's interface
	 */
	// 활성화가 되었을 때 어떤 동작을 할 것인지에 대한 함수
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;


	// PURE_VIRTUAL : 인터페이스 느낌.
	/**
	 * interface
	 */
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) PURE_VIRTUAL(UGameFeatureAction_WorldActionBase::AddToWorld, );
};