// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "LyraQuickBarComponent.generated.h"

class ULyraEquipmentManagerComponent;
class ULyraEquipmentInstance;
class ULyraInventoryItemInstance;

/**
 * HUD의 QuckBar를 생각하면 된다:
 * - 흔히 MMORPG에서는 ShortCut HUD를 연상하면 된다
 *
 * 해당 Component는 ControllerComponent로서, PlayerController에 의해 조작계 중 하나로 생각해도 된다
 * - HUD(Slate)와 Inventory/Equipment(Gameplay)의 다리(Bridge) 역활하는 Component로 생각하자
 * - 해당 Component는 Lyra의 HUD 및 Slate Widget을 다루면서 다시 보게될 예정이다
 */
UCLASS()
class LYRAPRAC_API ULyraQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()
	
public:
	ULyraQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	* ControllerComponent interface
	*/
	virtual void BeginPlay() override;

	/**
	* member methods
	*/

	ULyraEquipmentManagerComponent* FindEquipmentManager() const;
	void UnequipItemInSlot();
	void EquipItemInSlot();

	UFUNCTION(BlueprintCallable)
	void AddItemToSlot(int32 SlotIndex, ULyraInventoryItemInstance* Item);

	UFUNCTION(BlueprintCallable, Category = "Lyra")
	void SetActiveSlotIndex(int32 NewIndex);

	/** HUD QuickBar Slot 갯수 */
	UPROPERTY()
	int32 NumSlots = 3;

	/** HUD QuickBar Slot 리스트 */
	UPROPERTY()
	TArray<TObjectPtr<ULyraInventoryItemInstance>> Slots;

	/** 현재 활성화된 Slot Index (아마 Lyra는 딱 한개만 Slot이 활성화되는가보다?) */
	UPROPERTY()
	int32 ActiveSlotIndex = -1;

	/** 현재 장착한 장비 정보 */
	UPROPERTY()
	TObjectPtr<ULyraEquipmentInstance> EquippedItem;
};
