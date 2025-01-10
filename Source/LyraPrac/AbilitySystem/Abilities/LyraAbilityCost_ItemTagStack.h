// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraAbilityCost.h"
#include "LyraAbilityCost_ItemTagStack.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Item Tag Stack"))
class LYRAPRAC_API ULyraAbilityCost_ItemTagStack : public ULyraAbilityCost
{
	GENERATED_BODY()

public:
	ULyraAbilityCost_ItemTagStack();

	virtual bool CheckCost(const ULyraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const ULyraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	/**
	 * how much of the tag spend:
	 * - FScalableFloat is scaled by Curve with base float value
	 * - Curve is normally indexed by ability level
	 */
	// 비용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Costs)
	FScalableFloat Quantity;

	// 가져올 태그 비용에 대한 태그 이름
	/** gameplay tag combined with the cost */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Costs)
	FGameplayTag Tag;

	/** failure identifier with gameplay-tag */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Costs)
	FGameplayTag FailureTag;
};
