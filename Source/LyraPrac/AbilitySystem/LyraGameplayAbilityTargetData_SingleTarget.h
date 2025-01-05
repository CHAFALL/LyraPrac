#pragma once

#include "Abilities/GameplayAbilityTargetTypes.h"
#include "LyraGameplayAbilityTargetData_SingleTarget.generated.h"

USTRUCT()
struct FLyraGameplayAbilityTargetData_SingleTargetHit : public FGameplayAbilityTargetData_SingleTargetHit
{
	GENERATED_BODY()
public:
	FLyraGameplayAbilityTargetData_SingleTargetHit()
		: CartridgeID(-1)
	{}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FLyraGameplayAbilityTargetData_SingleTargetHit::StaticStruct();
	}

	/** 탄약 ID (카트리지) */
	UPROPERTY()
	int32 CartridgeID;
};