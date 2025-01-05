// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraPrac/AbilitySystem/Abilities/LyraGameplayAbility.h"
#include "LyraGameplayAbility_FromEquipment.generated.h"

class ULyraEquipmentInstance;
/**
 * 
 */
UCLASS()
class LYRAPRAC_API ULyraGameplayAbility_FromEquipment : public ULyraGameplayAbility
{
	GENERATED_BODY()
public:
	ULyraEquipmentInstance* GetAssociatedEquipment() const;
};
