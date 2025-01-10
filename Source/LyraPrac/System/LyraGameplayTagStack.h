// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "LyraGameplayTagStack.generated.h"

/**
 * Represents one stack of a gameplay tag (tag + count)
 * : for example, Ammo is representative example for GameplayTagStack
 */
// 아이템을 장착할 때마다 플레이어가 방어력도 오르고 공격력도 오를 수 있는데
// 그런 모든 스탯 관리를 여기서 해줌.
USTRUCT(BlueprintType)
struct FLyraGameplayTagStack
{
	GENERATED_BODY()

	FLyraGameplayTagStack() {}
	FLyraGameplayTagStack(FGameplayTag InTag, int32 InStackCount)
		: Tag(InTag)
		, StackCount(InStackCount)
	{}

	UPROPERTY()
	FGameplayTag Tag; // 탄창에 맞는 게임태그

	UPROPERTY()
	int32 StackCount = 0; // 탄창 수
};

/** container of LyraGameplayTagStack */
USTRUCT(BlueprintType)
struct FLyraGameplayTagStackContainer
{
	GENERATED_BODY()

	FLyraGameplayTagStackContainer() {}

	/** add/remove stack count by gameplay-tag */
	void AddStack(FGameplayTag Tag, int32 StackCount);
	void RemoveStack(FGameplayTag Tag, int32 StackCount);

	/** get the count by the gameplay tag */
	int32 GetStackCount(FGameplayTag Tag) const
	{
		return TagToCountMap.FindRef(Tag);
	}

	/** whether gameplay tag exists in LyraGameplayTagStackContainer */
	bool ContainsTag(FGameplayTag Tag) const
	{
		return TagToCountMap.Contains(Tag);
	}

	/** a list of gameplay tag stacks */
	UPROPERTY()
	TArray<FLyraGameplayTagStack> Stacks;

	// 스킬이라던가 스탯을 사용하는 경우는 한 프레임에서도 여러번 발동될 수 있을 정도로
	// 속도가 굉장히 중요한 작업이어서
	// Stacks에도 저장을 하되 TagToCountMap에도 저장을 함 (태그로 바로 찾을 수 있게 하기 위함.)
	/**
	 * LUT(Look-up table) to accelerate gameplay tag stack to query [GameplayTag -> Count]
	 * - we also use this LUT to find existance for corresponding gameplay tag
	 */
	TMap<FGameplayTag, int32> TagToCountMap;
};


// 그럼 이 TagStack은 누가 관리를 할까?
// -> 어빌리티 시스템을 정의하고 Weapon에 대한 시스템을 정의했던 곳이 어디?
// 바로 ItemInstance로 ㄱㄱ