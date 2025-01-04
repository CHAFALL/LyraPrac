// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraPrac/Cosmetics/LyraCosmeticAnimationTypes.h"
#include "LyraPrac/Equipment/LyraEquipmentInstance.h"
#include "LyraWeaponInstance.generated.h"

/**
 * 
 */
UCLASS()
class LYRAPRAC_API ULyraWeaponInstance : public ULyraEquipmentInstance
{
	GENERATED_BODY()

public:
	ULyraWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Weapon에 적용할 AnimLayer를 선택하여 반환 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Animation)
	TSubclassOf<UAnimInstance> PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const;

	/** Weapon에 Equip/Unequip에 대한 Animation Set 정보를 들고 있다 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FLyraAnimLayerSelectionSet EquippedAnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FLyraAnimLayerSelectionSet UnequippedAnimSet;
};


// Cosmetic에선 메시를 선택한 반면
// 여기선 애니메이션 레이어를 골라옴.