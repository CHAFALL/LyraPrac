// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "UIExtensionSystem.h"
#include "GameFeatureAction_WorldActionBase.h"
#include "GameFeatureAction_AddWidgets.generated.h"

/** forward declarations */
struct FComponentRequestHandle;

// 둘다 TSoftClassPtr을 보아 경로이다
// -> 당연히 로딩한 뒤에 장착을 진행
// 위젯은 그냥 깡으로 들고 있기에는 너무 무거워서 대부분 이렇게 함.

// 이 GameplayTag 에다가 해당 레이아웃을 장착해줘
/** HUD의 Layout 요청 */
USTRUCT()
struct FLyraHUDLayoutRequest
{
	GENERATED_BODY()

	/** UI의 레이아웃으로 CommonActivatableWidget을 사용 */
	UPROPERTY(EditAnywhere, Category = UI, meta = (AssetBundles = "Client"))
	TSoftClassPtr<UCommonActivatableWidget> LayoutClass;

	/** 앞서 보았던 PrimaryGameLayout의 LayerID를 의미 */
	UPROPERTY(EditAnywhere, Category = UI)
	FGameplayTag LayerID;
};

// 위에서 결정한 레이아웃 안에서 또 Gameplay 태그를 찾고 거기에 위젯을 붙임
USTRUCT()
struct FLyraHUDElementEntry
{
	GENERATED_BODY()

	/** LyraHUDLayout 위에 올릴 대상이 되는 Widget Class */
	UPROPERTY(EditAnywhere, Category = UI, meta = (AssetBundles = "Client"))
	TSoftClassPtr<UUserWidget> WidgetClass;

	/** SlotID는 LyraHUDLayoutRequest에 올린 LayoutClass에 정의된 Slot(GameplayTag)를 의미 */
	UPROPERTY(EditAnywhere, Category = UI)
	FGameplayTag SlotID;
};

/**
 * 
 */
UCLASS()
class LYRAPRAC_API UGameFeatureAction_AddWidgets : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()
	
public:
	// GameFeature 방식에서는 FPerContextData를 꼭 들고 있음
	// 왜냐 이게 없으면 관리 hard (GameFeature 시스템은 여러 World, 여러 Context에서 동작할 수 있어서 각 Context마다 별도의 상태와 데이터를 관리해야 됨....)
	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequests;
		TArray<TWeakObjectPtr<UCommonActivatableWidget>> LayoutsAdded;

		/** Lyra에서 HUDElement는 UIExtension으로 관리된다. */
		TArray<FUIExtensionHandle> ExtensionHandles;
	};

	void AddWidgets(AActor* Actor, FPerContextData& ActiveData);
	void RemoveWidgets(AActor* Actor, FPerContextData& ActiveData);

	/**
	 * UGameFeatureAction's interface
	 */
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

	/**
	 * UGameFeatureAction_WorldActionBase's interface
	 */
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;

	void HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);


	// 어떤 Action을 활성화 했을 때 현재 월드 정보라던가 GameFeature에 대한 현재 정보를
	// 같이 넘김.
	// 그러면 엔진 상에서 다양한 인스턴스가 있고 거기에 대해서 구분을 할 때
	// key를 넣고 value를 찾게 되면 해당 action에 맞는 데이터가 딱 저장이 됨
	// (input편과 유사)
	/** GFA Add/Remove 상태 관리 */
	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

	/**
	 * GFA_AddWidget은 형태를 정의하는 Layout과 Layout 위에 올릴 Widget 객체로 Widgets으로 구성된다
	 */
	 // 어떤 Layout을 장착할 것인지
	UPROPERTY(EditAnywhere, Category = UI)
	TArray<FLyraHUDLayoutRequest> Layout;

	// UI Extension이 A, B, C 태그가 있으면 A 태그에는 어떤 위젯을 장착해라 뭐 이런 것을 시키는 역할을 함.
	// 위치가 같더라도 캐릭터 종류에 따라서 생김새가 바뀔 수도 있잖아~ 이를 대비함.
	UPROPERTY(EditAnywhere, Category = UI) 
	TArray<FLyraHUDElementEntry> Widgets; 
};



/**
*	전체적인 흐름
*	1.GameFeature가 활성화되면 AddToWorld가 호출됨
	2.ComponentManager에 핸들러를 등록
	3.HUD의 상태 변경에 따라 HandleActorExtension이 호출됨
	4.HandleActorExtension에서 상황에 맞게 AddWidgets 또는 RemoveWidgets을 호출
	5.위젯들이 UI에 추가되거나 제거됨
*
*/