// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "LyraAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class LYRAPRAC_API ULyraAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	ULyraAssetManager();

	static ULyraAssetManager& Get();

	/**
	* UAssetManager's interfaces
	*/
	// 함수가 시작되는 시점.
	virtual void StartInitialLoading() final;

	/**
	 * 해당 함수는 FScopeLogTime을 사용할지 않을지 유무 판단을 위해 사용
	 * - 굳이 클론 코딩할 필요없지만, 알아두면 좋은 점들이 있으니 해보자
	 */
	static bool ShouldLogAssetLoads();

	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);

	/**
	 * GetAsset은 정적 로딩으로 BP Class와 Object 로딩
	 * - 주목해야 할 부분은 bKeepInMemory
	 */
	// 템플릿이어서 헤더에 정의해줘야 됨. (템플릿은 컴파일 단계에서 코드로 만들어지기 때문)
	template <typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	template <typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	/** [THREAD-SAFE] 메모리에 로딩된 에셋 캐싱 */
	void AddLoadedAsset(const UObject* Asset);

	// GC의 대상
	// LoadedAssets에 추가를 해줌?
	// 로딩을 하고 메모리에 올려놓으면 얘가 마음대로 삭제가 되면 안되겠지? 그래서 그런 것들을 저장해둘려고
	// 여기에 추가를 하면 계속 사용하고 있다고 인지해서 GC에 안걸림. (캐싱 용도 느낌.)
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	// Object 단위 Locking , 열쇠느낌으로 열쇠를 든 애만 문을 열고 들어올 수 있는 느낌.
	FCriticalSection SyncObject;
};

template <typename AssetType>
AssetType* ULyraAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepsInMemory)
{
	AssetType* LoadedAsset = nullptr;
	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		// 로딩이 되어있다? -> 바로 가져옴
		// 로딩이 안되어 있다 -> Null
		LoadedAsset = AssetPointer.Get();
		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}

		if (LoadedAsset && bKeepsInMemory)
		{

			// 왜 아래와 같이 안하고 Get().AddLoadedAsset(Cast<UObject>(LoadedAsset)); 이렇게 했을까?
			// 아직 에디터가 켜지기도 전에는 쓰레드가 Safe하지 않음.. (StartInitialLoading에 디버그를 찍어보면 에디터가 커지기 전에 찍히는 것을 알 수 있음, 그래서 락을 걸어줌.)
			//LoadedAssets.Add(LoadAsset);

			// 여기서 AddLoadAsset은 메모리에 상주하기 위한 장치라고 생각하면 됨:
			// - 한번 등록되면 직접 내리지 않는한 Unload가 되지 않음 (== 캐싱)
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}

	return LoadedAsset;
}

template <typename AssetType>
TSubclassOf<AssetType> ULyraAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();
	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetPointer.ToString());
		}

		if (LoadedSubclass && bKeepInMemory)
		{
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}
