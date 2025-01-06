// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraGameplayCueManager.h"
#include "AbilitySystemGlobals.h"
#include "GameplayCueSet.h"
#include "Engine/AssetManager.h"


ULyraGameplayCueManager::ULyraGameplayCueManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

ULyraGameplayCueManager* ULyraGameplayCueManager::Get()
{
	return Cast<ULyraGameplayCueManager>(UAbilitySystemGlobals::Get().GetGameplayCueManager());

}

const FPrimaryAssetType UFortAssetManager_GameplayCueRefsType = TEXT("GameplayCueRefs");
const FName UFortAssetManager_GameplayCueRefsName = TEXT("GameplayCueReferences");
const FName UFortAssetManager_LoadStateClient = FName(TEXT("Client"));


// Gameplay Cue 같은 경우 Effect를 담당하는 시스템이다보니 서버에서 알 필요가 없음
// (서버에서는 렌더링 관련된 모든 정보를 다 배제함.)
// -> 그래서 AddBundleAssetsTruncated 이용.
void ULyraGameplayCueManager::RefreshGameplayCuePrimaryAsset()
{
	TArray<FSoftObjectPath> CuePaths;
	UGameplayCueSet* RuntimeGameplayCueSet = GetRuntimeCueSet();
	if (RuntimeGameplayCueSet)
	{
		RuntimeGameplayCueSet->GetSoftObjectPaths(CuePaths);
	}

	// 새로 추가된 에셋의 경로를 CuePaths에 추가하여!
	FAssetBundleData BundleData;
	BundleData.AddBundleAssetsTruncated(UFortAssetManager_LoadStateClient, CuePaths);

	// PrimaryAssetId를 고정된 이름으로 설정하여 (const로 설정되어 있음)
	FPrimaryAssetId PrimaryAssetId = FPrimaryAssetId(UFortAssetManager_GameplayCueRefsType, UFortAssetManager_GameplayCueRefsName);

	// 에셋 매니저에 추가 진행
	UAssetManager::Get().AddDynamicAsset(PrimaryAssetId, FSoftObjectPath(), BundleData);
}
