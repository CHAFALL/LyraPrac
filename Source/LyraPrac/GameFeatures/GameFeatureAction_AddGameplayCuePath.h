#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeatureAction_AddGameplayCuePath.generated.h"

UCLASS()
class UGameFeatureAction_AddGameplayCuePath : public UGameFeatureAction
{
	GENERATED_BODY()
public:
	UGameFeatureAction_AddGameplayCuePath();

	/** GameplayCueNotify 에셋 경로 추가 */
	UPROPERTY(EditAnywhere, Category = "GameFeature|GameplayCues")
	TArray<FDirectoryPath> DirectoryPathsToAdd;
};



// 신기하게 얘는 월드베이스를 상속 받지 않고 있다.
// why? GameFeature의 GameplayCue의 경우 모든 월드에서 다 알고 있는 것이 더 나아서 그럼


// 지금까지 Action을 만들었을 때 어디선가는 액티브를 시켜줘야 했다. (참고. ULyraExperienceManagerComponent::OnExperienceFullLoadCompleted())
// 근데 GameplayCue의 경우 예외다!!!
// -> AOS 모드에서 FPS 모드로 바뀌었을 때 어느 세월에 스캔 등록해서 어느 세월에 로딩할래!! (심지어 AOS의 스킬 이펙트를 FPS에서 안 쓴다는 보장이 있니?)
// 기존의 Action들은 게임 모드, 즉 Lyra관점에서 보면 Experience에 종속적이었다. 근데 GameplayCue는 모드가 바뀐다고 안 쓴다는 보장도 없고 탐색 속도와 등록하는 것을 생각해보면 너무 느림!
// 그래서 기존의 방식대로 하지 않고 Lyra에서는 Observer 패턴을 이용!! 
// (게임이 실행이 되어서 GameFeature 자체가 스캔/등록되는 절차(레지스트링)가 있을 것인데 그때 이 path에 있는 것들을 자동으로 전부 추가해버림)
// 구현 과정은 Policy 참고