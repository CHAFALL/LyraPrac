// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_ExperienceReady.generated.h"

/** BP를 통한 UFunction으로 바인딩하는 Multicast Delegate: Dynamic! */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExperienceReadyAsyncDelegate);

/** forward declarations */
class AGameStateBase;
class ULyraExperienceDefinition;


UCLASS()
class LYRAPRAC_API UAsyncAction_ExperienceReady : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UAsyncAction_ExperienceReady(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** UAsyncAction_ExperienceReady를 생성하고 기다리는 BP 호출 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static UAsyncAction_ExperienceReady* WaitForExperienceReady(UObject* WorldContextObject);

	/**
	 * UBlueprintAsyncActionBase interface
	 */
	virtual void Activate() override;

	/**
	 * Step1 - Step4
	 */
	void Step1_HandleGameStateSet(AGameStateBase* GameState);
	void Step2_ListenToExperienceLoading(AGameStateBase* GameState);
	void Step3_HandleExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience);
	void Step4_BroadcastReady();

	/** BlueprintAssignable은 BP상에서 할당 가능한 변수로 정의한다 */
	UPROPERTY(BlueprintAssignable)
	FExperienceReadyAsyncDelegate OnReady;

	/** WorldPtr을 단순 로직상 캐싱하는 용도 */
	TWeakObjectPtr<UWorld> WorldPtr;
};

// Experience가 로딩이 완료가 되었는지를 체크하는 것이 얘의 목표.
// 블루프린트를 사용할 때 현재 시점에서 Experience가 로딩이 되었는지 안되었는지를 알 수 없기 때문에
// 이를 해결하기 위한 Action이라고 보면 됨.