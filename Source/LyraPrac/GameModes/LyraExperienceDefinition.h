// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraExperienceDefinition.generated.h"

class ULyraExperienceActionSet;
class UGameFeatureAction;
class ULyraPawnData;

// Good
/**
 * 
 */
UCLASS()
class LYRAPRAC_API ULyraExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	ULyraExperienceDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * member variables
	 */
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TObjectPtr<ULyraPawnData> DefaultPawnData;

	/**
	 * 해당 property는 단순히 마킹 및 기억용으로 남겨둔다
	 * - 게임 모드에 따른 GameFeature plugin을 로딩하는데 이에 대한 연결고리고 생각하면 된다
	 * - 후일 ShooterCore 관련 Plugin 작업할 경우, 진행하게 될 예정이다
	 */
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;

	/** ExperienceActionSet은 UGameFeatureAction의 Set이며, Gameplay 용도에 맞게 분류의 목적으로 사용한다 */
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<TObjectPtr<ULyraExperienceActionSet>> ActionSets;

	/** 일반적인 GameFeatureAction으로서 추가 */
	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;

};


// Lyra에서는 매핑 컨텍스트도 게임의 용도에 따라 내가 게임 패드냐 키보드냐 또는 모드에 따라서 바뀔 수 있으니깐 그것도 experience definition의 용도에 맞춰서 동적으로 설정!! 
// -> 그걸 gameFeature에서 함