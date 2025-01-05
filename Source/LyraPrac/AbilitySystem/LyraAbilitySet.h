// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayAbilitySpec.h"
#include "LyraAbilitySet.generated.h"

class ULyraAbilitySystemComponent;
class ULyraGameplayAbility;

// 어빌리티과 태그를 매칭시키기 위해 만듬.
// why? 어빌리티 시스템에서는 태그를 이용해서 어빌리티를 가져올 수 있음
/**
 * GameplayAbility의 Wrapper 클래스
 * - 추가적인 커스터마이징이 가능함
 */
USTRUCT(BlueprintType)
struct FLyraAbilitySet_GameplayAbility
{
	GENERATED_BODY()
public:
	/** 허용된 GameplayAbility */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULyraGameplayAbility> Ability = nullptr;

	/** Input 처리를 위한 GameplayTag */
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;

	/** Ability의 허용 조건 (Level) */
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;
};

USTRUCT(BlueprintType)
struct FLyraAbilitySet_GrantedHandles
{
	GENERATED_BODY()

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void TakeFromAbilitySystem(ULyraAbilitySystemComponent* LyraASC);

protected:
	/** 허용된 GameplayAbilitySpecHandle(int32) */
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
};

/**
 * 
 */
UCLASS()
class LYRAPRAC_API ULyraAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	ULyraAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** ASC에 허용가능한 Ability를 추가한다 */
	void GiveToAbilitySystem(ULyraAbilitySystemComponent* LyraASC, FLyraAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr);

	/** 허용된 GameplayAbilities */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TArray<FLyraAbilitySet_GameplayAbility> GrantedGameplayAbilities;
};

// 그럼 이건 누가 가지고 있을까? - PawnData에서 관리