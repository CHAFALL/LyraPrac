// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraGameplayAbility.h"
#include "GameplayAbilitySpec.h"
#include "LyraAbilityCost.generated.h"

/**
 * 
 */
// 중요 - 3개의 키워드
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class LYRAPRAC_API ULyraAbilityCost : public UObject
{
	GENERATED_BODY()
	
public:
	ULyraAbilityCost();

	/**
	 * CheckCost and ApplyCost function signature come from UGameAbility's CheckCost and ApplyCost
	 * - You can think LyraAbilityCost as managable-unit to check/apply cost for GameplayAbility
	 */
	virtual bool CheckCost(const ULyraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
	{
		return true;
	}

	virtual void ApplyCost(const ULyraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
	{
	}
};
