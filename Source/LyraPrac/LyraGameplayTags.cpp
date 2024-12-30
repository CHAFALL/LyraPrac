#include "LyraGameplayTags.h"
#include "LyraLogChannels.h"
#include "GameplayTagsManager.h"

FLyraGameplayTags FLyraGameplayTags::GameplayTags;

// 누군가는 이 InitializeNativeTags를 호출해줘야겠지? (Asset Manager의 StartInitialLoading에서 해줄 것임.)
void FLyraGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	GameplayTags.AddAllTags(Manager);
}

void FLyraGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	// AddNativeGameplayTag를 통해 에디터에 태그를 등록하고 C++에서 컨트롤하기 위해 OutTag에다가 넣어줌
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}

void FLyraGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	/**
	 * GameFrameworkComponentManager init state tags
	 */
	AddTag(InitState_Spawned, "InitState.Spawned", "1: Actor/Component has initially spawned and can be extended");
	AddTag(InitState_DataAvailable, "InitState.DataAvailable", "2: All required data has been loaded/replicated and is ready for initialization");
	AddTag(InitState_DataInitialized, "InitState.DataInitialized", "3: The available data has been initialized for this actor/component, but it is not ready for full gameplay");
	AddTag(InitState_GameplayReady, "InitState.GameplayReady", "4: The actor/component is fully ready for active gameplay");

	/**
	 * Enhanced Input Tags
	 */
	AddTag(InputTag_Move, "InputTag.Move", "");
	AddTag(InputTag_Look_Mouse, "InputTag.Look.Mouse", "");
}
