// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "LyraInventoryItemDefinition.h"
#include "LyraInventoryFragment_SetStats.generated.h"

/**
 * 
 */
UCLASS()
class LYRAPRAC_API ULyraInventoryFragment_SetStats : public ULyraInventoryItemFragment
{
	GENERATED_BODY()
public:
	virtual void OnInstanceCreated(ULyraInventoryItemInstance* Instance) const override;

	/** InitialItemStats gives constructor's parameters for LyraGameplayTagStackContainer */
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TMap<FGameplayTag, int32> InitialItemStats; // 해당 item에다가 여러 스탯을 넣을 수 있음.
};

// 스탯을 fragment에다가 정의를 했다는 것은 누군가는 설정을 해줘야 된다는 뜻이다.
// (복습) 장착 아이템에 대한 Fragment를 만들었을 때는 Equipment Manager의 AddEntry함수에서 EquipmentDefinition을 가져와서 세팅을 진행했었음
// 
// 이 스탯도 Fragment이다보니 누군가는 어디다가 정의를 해줘야됨. 



