// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "LyraPawnExtensionComponent.generated.h"

class ULyraPawnData;

/**
 * 초기화 전반을 조정하는 컴포넌트
 */

/*
 * Pawn 구조:
 * └─ Pawn
 *    ├─ PawnExtensionComponent (초기화 관리자)
 *    └─ 게임플레이 컴포넌트들
 *       └─ HeroComponent
*/

// PawnExtensionComponent이 다른 놈들보다 젤 먼저 초기화가 되는게 보장이 되어야 됨
// - 종속성을 제거하면서 PawnExtensionComponent가 젤 먼저 생성되게 하는게 사실 쉽진 않음.
// -> 사실 쉽게 하려면 beginPlay 같은데나 tick에서 돌리는 것인데 라이라에서는 tick을 다 막아놧음
// 엔진 구조상 네이티브 C++에다가 선언한 것이 먼저 발동(생성) 되고 그 다음에 블루프린트 안의 SCS 부분을 생성함
// 그래서 관리자는 네이티브C++로 만들고 그 이후로 우리가 관리할 컴포넌트들은 SCS로 만들어주면 해결!!

// InitState를 통해 부품 순서를 보장해주고 모든 부품이 모두 준비가 되면
// 그 다음부터 초기화가 진행되도록 하고 싶은데 이를 가능하게 해줌.
// 여기선 부품이 Hero뿐이라 굳이? 인 느낌이지만 라이라에서 예시를 보여준 것임.

// 근데 그러면 extension이랑 부품들이랑 종속성이 생기지 않니?
// 이것도 라이라가 해결

// IGameFrameworkInitStateInterface: 우리가 만든 RegisterInitState 애네에 대한 초기화를 편리하게 관리할 수 있도록 만들어준 인터페이스



UCLASS()
class LYRAPRAC_API ULyraPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
	
public:
	ULyraPawnExtensionComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Feature name은 GameFrameworkComponentManager에서 컴포넌트들을 식별하고 관리하는데 사용됩니다.
	// 여기서 정의까지 한번에 하면 에러 뜸 CPP에서 정의할 것!
	/** FeatureName 정의 */
	static const FName NAME_ActorFeatureName;

	/**
	* member methods
	*/
	static ULyraPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<ULyraPawnExtensionComponent>() : nullptr); }
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	void SetPawnData(const ULyraPawnData* InPawnData);
	void SetupPlayerInputComponent();
	
	/**
	* UPawnComponent interfaces
	 */
	virtual void OnRegister() final;
	virtual void BeginPlay() final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;

	/**
	 * IGameFrameworkInitStateInterface
	 */
	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	virtual void CheckDefaultInitialization() final;

	// PawnExtension 컴포넌트는 모든 컴포넌트들에 대해서 초기화를 관리하는 놈이기 때문에
	// 데이터를 핸들링 하는 것은 없음.

	/**
	 * Pawn을 생성한 데이터를 캐싱 - 이렇게 해야 hero 컴포넌트라던가 다른 애들도 정상적으로 잘 사용 가능
	 */
	UPROPERTY(EditInstanceOnly, Category = "Lyra|Pawn")
	TObjectPtr<const ULyraPawnData> PawnData;
};

// 핵심은
// 강제 업데이트랑
// OnActorInitStateChanged

// Possess에서 해주는 역할
// 입력과 관련된 것은 플레이어컨트롤러로 가는데, 우리는 캐릭터를 조절해야하잖아
// 그래서 Possess라는 것이 진행이 되고 나면 내부적으로 Delegate같은 것이 일어나면서 캐릭터로 입력이 넘어감
// 그런 것들을 알 수 있는 함수 호출 시점이 SetupPlayerInputComponent임.

// Possess가 되고 난 뒤에 플레이어 컨트롤러에 대한 입력을 캐릭터로 넘기는 것에 대한 설정이 완료되는 시점이
// SetupPlayerInputComponent임.
// 이걸 이용할 것임.