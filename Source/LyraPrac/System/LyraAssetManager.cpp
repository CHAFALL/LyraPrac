// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraAssetManager.h"
#include "LyraPrac/LyraLogChannels.h"

ULyraAssetManager::ULyraAssetManager()
{
}

ULyraAssetManager& ULyraAssetManager::Get()
{
	check(GEngine);

	// 우리는 AssetManager를 UEngine의 GEngine의 AssetManager의 Class를 오버라이드 했기 때문에, GEngine에 Asset Manager가 있음
	// 유일성 보장.
	if (ULyraAssetManager* Singleton = Cast<ULyraAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogLyra, Fatal, TEXT("invalid AssetManagerClassname in DefaultEngine.ini(project settings); it must be LyraAssetManager"));

	// 위의 UE_LOG의 Fatal로 인해, Crash 나기 때문에 도달하지 않지만 컴파일을 위해 더미로 리턴
	return *NewObject<ULyraAssetManager>();
}

// Development로 디버깅을 하면 디버깅과 관련된 것들을 최소한으로 저장해서 로딩이 빠름
// 하지만 단점으로 안 찝어주는 것이 있어서 PRAGMA_DISABLE_OPTIMIZATION, PRAGMA_ENABLE_OPTIMIZATION를 이용해봄,
PRAGMA_DISABLE_OPTIMIZATION
void ULyraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
}
PRAGMA_ENABLE_OPTIMIZATION

bool ULyraAssetManager::ShouldLogAssetLoads()
{
	const TCHAR* CommandLineContent = FCommandLine::Get();
	static bool bLogAssetLoads = FParse::Param(CommandLineContent, TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

// 중요! (동기식)
// 메모리 채로 들고 있으면 부담이 되니 FSoftObjectPath를 통해 경로만.
UObject* ULyraAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	// 해당 함수를 따로 만든 이유는 'synchronous load asset이 불필요하게 하는 것이 없는지 확인하기 위함'
	if (AssetPath.IsValid())
	{
		// FScopeLogTime을 확인해보자:
		// 이걸 통해서 얘는 동기 로딩을 하면 안되겠구나를 찾아낼 수도 있음.
		TUniquePtr<FScopeLogTime> LogTimePtr;
		if (ShouldLogAssetLoads())
		{
			// 단순히 로깅하면서, 초단위로 로깅 진행
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("synchronous loaded assets [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		// 여기서 두가지 분기:
		// 1. AssetManager가 있으면, AssetManager의 StreamableManager를 통해 정적 로딩
		// 2. 아니면, FSoftObjectPath를 통해 바로 정적 로딩
		if (UAssetManager::IsValid())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);
		}

		// if asset manager is not ready, use LoadObject()
		// - 슥 보면, StaticLoadObject가 보인다: 
		// - 참고로, 항상 StaticLoadObject하기 전에 StaticFindObject를 통해 확인하고 실패하면 진짜 로딩함
		// 다만 이건 매우 느리기 때문에 웬만하면 이건 안 쓰도록 해야됨.
		return AssetPath.TryLoad();
	}

	// LoadSynchronous랑 TryLoad의 공통점 중 하나: 한번 했던 로딩을 또 할 필요가 x하므로
	// (내부적으로 어느정도 캐싱을 해둬서) 로딩이 진짜 안되어 있을 때만 로딩을 하고 로딩이 되어있으면 캐싱이 되어있는 값을 가져옴 

	return nullptr;
}

void ULyraAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock Lock(&SyncObject);
		LoadedAssets.Add(Asset);
	}
}
