// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraGameplayAbility.h"
#include "LyraAbilityCost.h"

ULyraGameplayAbility::ULyraGameplayAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ActivationPolicy = ELyraAbilityActivationPolicy::OnInputTriggered;
	// 액터 단위로 인스턴스가 생성되기 때문에 CDO임에도 인스턴스 쓰는 것은 문제가 있으므로
	// 아래와 같이 변경 필요.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool ULyraGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags) || !ActorInfo)
	{
		return false;
	}

	// verify AdditionalCosts defined in LyraGameplayAbility to activate GameplayAbility:
	for (TObjectPtr<ULyraAbilityCost> AdditionalCost : AdditionalCosts)
	{
		if (AdditionalCost != nullptr)
		{
			if (!AdditionalCost->CheckCost(this, Handle, ActorInfo, OptionalRelevantTags))
			{
				return false;
			}
		}
	}

	// all cost requipements are meet! ready to activate!
	return true;
}

void ULyraGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);
	check(ActorInfo);

	// pay any additional cost
	for (TObjectPtr<ULyraAbilityCost> AdditionalCost : AdditionalCosts)
	{
		if (AdditionalCost != nullptr)
		{
			AdditionalCost->ApplyCost(this, Handle, ActorInfo, ActivationInfo);
		}
	}
}
