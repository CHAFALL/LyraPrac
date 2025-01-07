// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameUIPolicy.generated.h"

/** forward declarations */
class UPrimaryGameLayout;
class UCommonLocalPlayer;

USTRUCT()
struct FRootViewportLayoutInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> LocalPlayer = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UPrimaryGameLayout> RootLayout = nullptr;

	UPROPERTY(Transient)
	bool bAddedToViewport = false;

	bool operator==(const ULocalPlayer* OtherLocalPlayer) const { return LocalPlayer == OtherLocalPlayer; }

	FRootViewportLayoutInfo() {}
	FRootViewportLayoutInfo(ULocalPlayer* InLocalPlayer, UPrimaryGameLayout* InRootLayout, bool bIsInViewport)
		: LocalPlayer(InLocalPlayer)
		, RootLayout(InRootLayout)
		, bAddedToViewport(bIsInViewport)
	{}
};


/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class COMMONGAME_API UGameUIPolicy : public UObject
{
	GENERATED_BODY()
	
public:
	UPrimaryGameLayout* GetRootLayout(const UCommonLocalPlayer* LocalPlayer) const;
	TSubclassOf<UPrimaryGameLayout> GetLayoutWidgetClass(UCommonLocalPlayer* LocalPlayer);

	void CreateLayoutWidget(UCommonLocalPlayer* LocalPlayer);
	void AddLayoutToViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);
	void RemoveLayoutFromViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);

	void NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer);

public:
	/** LocalPlayer에 바인딩된 UI의 Layout으로 생각하면 된다 (아직 의미가 모호할 수 있는데, 하나씩 구현해보면서, 어떤 느낌인가 더 와닫을 것이다) */
	// TSoftClassPtr인걸 보니 경로
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UPrimaryGameLayout> LayoutClass;

	/** 보통 싱글 게임에서는 LocalPlayer-PrimaryGameLayout 하나만 있겠지만, 멀티플레이의 경우, 복수개 가능하다 (리플레이를 생각해보자) */
	// ex. 메탈슬러그 2p 
	// Layout이랑 LocalPlayer랑 맵핑
	UPROPERTY(Transient)
	TArray<FRootViewportLayoutInfo> RootViewportLayouts;
};

// 이 UPrimaryGameLayout은 플레이어가 시작하자마자 바로 세팅 되어야 겠지?
// -> 왜냐하면 UPrimaryGameLayout가 세팅이 안되어 있으면 나머지 UI를 구축 불가
// 그래서 굉장히 빠른 시점에 해야됨!!
// 그때가 언제?? -> LocalPlayer가 PlayerController에 등록이 되는 순간. 
// (그걸 확인할 수 있는 곳은?? -> GameInstance
// 우린 GameInstance를 상속받은 CommonGameInstance에서 해줄 것임)


/**
* UPrimaryGameLayout (RootLayout)
    ├── HUD Layer
    ├── Menu Layer
    ├── Modal Layer
    └── Other UI Layers...
*/

// GPT
/**
*	UI 레이아웃 정책 설정
	-어떤 상황에서 어떤 UI를 보여줄지 결정
	-UI 요소들의 표시/숨김 규칙 정의
	-UI 전환 방식 관리

	UI 상태 관리
	-게임 상태에 따른 UI 상태 변경
	-메뉴, HUD, 팝업 등의 표시 우선순위 설정
	-UI 스택 관리

	입력 처리 정책
	-UI가 활성화되었을 때의 입력 처리 방식 정의
	-UI 요소 간의 포커스 이동 규칙 설정

	UI 커스터마이제이션
	-기본 UI 동작을 커스터마이즈할 수 있는 정책 제공
	-프로젝트별 특수한 UI 요구사항 구현
*/