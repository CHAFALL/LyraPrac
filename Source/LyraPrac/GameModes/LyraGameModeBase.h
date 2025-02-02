// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LyraGameModeBase.generated.h"

// 전방 선언을 했으면 CPP에도 반드시 include해줘야 됨.

class ULyraPawnData;
class ULyraExperienceDefinition;

/**
 * 
 */
UCLASS()
class LYRAPRAC_API ALyraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALyraGameModeBase();
	
	// 극초반에 실행이 됨.
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	// GameState가 생성이 완료가 되고 난 후에 호출이 됨.
	// 즉, GameState가 생성이 되었다는 것은 GameState에서 Experience Manager 컴포넌트를 만들었겠지?
	// 즉, 예약 가능한 상태.
	virtual void InitGameState() final;

	// 우리의 작고 귀여운 친구를 가져오기 위해 오버라이딩
	/** GetDefaultPawnClassForController */
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) final;

	// 우리가 원하는 것은 experience 로딩이 완료가 된 후에 캐릭터가 스폰되길 원함
	// 그래서 스폰 관련 부분을 오버라이딩해서 experience 로딩이 완료되기 전까지 막아놓을 것임.
	// 로딩이 완료가 된 후에 restart라는 것을 통해 해당하는 부분들을 다시 호출해줄 것임.
	/** HandleStartingNewPlayer */
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) final;

	/** SpawnDefaultPawnAtTransform */
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) final;


	/**
	 * member methods
	 */
	void HandleMatchAssignmentIfNotExpectingOne();
	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId);
	bool IsExperienceLoaded() const;
	void OnExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience);
	const ULyraPawnData* GetPawnDataForController(const AController* InController) const;

};


// Implementation가 붙었다는 것은 블루프인트 네이티브 이벤트라는 것임
// 블루프린트에서 구현을 해도 되고 C++에서 구현을 해도 됨.


// 알아야 할 점 : 엔진을 수정하지 않고 오버라이딩 식으로 진행함
// 로딩이 완료되기전까지는 대기, 그 중간다리는 experience manager가 해줌.

// 라이라 방식 - 내가 뭔가를 했더니 문제가 있네? 그래서 찾아들어가보면 엔진에서 설정되어 있는 값이 있을 것이고
// 그걸 우리가 원하는 방식으로 바꾸기 위해 오버라이딩

// 압축 정리 - #4의 22분 부터





