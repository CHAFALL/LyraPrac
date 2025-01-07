// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameUIManagerSubsystem.generated.h"

/** forward declarations */
class UGameUIPolicy;
class UCommonLocalPlayer;

/**
 * 
 */
UCLASS(Abstract, config = Game)
class COMMONGAME_API UGameUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UGameUIManagerSubsystem(); //Subsystem의 경우 생성자 필수!

	void SwitchToPolicy(UGameUIPolicy* InPolicy);

	/**
	 * UGameInstanceSubsystem's interfaces
	 */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	/**
	 * UGameUIManagerSubsystem's interfaces
	 */
	virtual void NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer);

	const UGameUIPolicy* GetCurrentUIPolicy() const { return CurrentPolicy; }
	UGameUIPolicy* GetCurrentUIPolicy() { return CurrentPolicy; }


	// TObjectPtr니깐 메모리에 올라가 있는 값
	// 즉, CurrentPolicy는 DefaultUIPolicyClass에서 설정된 값을 넣어줄 것임
	// UGameUIPolicy 얘가 실질적으로 관리해주는 느낌.
	UPROPERTY(Transient)
	TObjectPtr<UGameUIPolicy> CurrentPolicy = nullptr;

	// TSoftClassPtr니깐 어떤 경로
	/**
	 * default UI policy를 생성할 class
	 * - 우리는 해당 클래스는 B_BttGameUIPolicy로 지정할 것이다
	 */
	UPROPERTY(Config, EditAnywhere)
	TSoftClassPtr<UGameUIPolicy> DefaultUIPolicyClass;
};

// UGameUIPolicy 얘가 실질적으로 관리해주는 느낌. (너 어떤 레이아웃 쓸건데?)
// -> Lyra : UI 시스템이 굉장히 사람마다 개발 방식도 다르고 굉장히 추상화 레벨이 높은 컨텐츠 중 하나이다.
// 그러니깐 너가 너만의 정책을 결정해! (대신에 기본 UIPolicy는 무조건 결정이 되어야 함.)


// 확장성 고려 Good (subSystem의 한계 고려)
// 만약에 UIPolicy 시스템을 사용하는 것이 아니라 진짜 매니저에서 모든 것을 관리하는 식으로 했다면
// 새로운 패턴을 만들 때마다 이 UIManagerSubsystem을 상속 받아서 만들어야 되는데
// Subsystem은 계속 상속 받다 보면 결국 맨 마지막 것밖에 못 쓰는 문제점이 있음(내부적으로 결국 제일 마지막 것만 생성해줌)
// 그러다 보니 굉장히 복잡해짐....(다중 패턴을 동시에 이용 불가..)
// 그래서 UIPolicy를 따로 쓰는 것임 - 여러개를 만들어 놓고 내가 쓰고 싶은거를 쓰면 됨!!