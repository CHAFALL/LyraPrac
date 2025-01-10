// Fill out your copyright notice in the Description page of Project Settings.

#include "LyraGameplayAbility_FromEquipment.h"
#include "LyraEquipmentInstance.h"
#include "LyraPrac/Inventory/LyraInventoryItemInstance.h"

ULyraEquipmentInstance* ULyraGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	// CurrentActorInfo의 AbilitySystemComponent와 CurrentSpecHandle을 활용하여, GameplayAbilitySpec을 가져옴:
	// - CurrentSpecHandle은 SetCurrentActorInfo() 호출할 때, Handle 값을 받아서 저장됨:
	// - CurrentSpecHandle와 CurrentActorInfo은 같이 함
	// - FindAbilitySpecFromHandle은 GiveAbility로 허용된 ActivatableAbilities를 순회하여 GameplayAbilitySpec을 찾아냄
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		// GameplayAbility_FromEquipment는 EquipmentInstance로부터 GiveAbility를 진행했으므로, SourceObject에 EquipmentInstance가 저장되어 있음
		return Cast<ULyraEquipmentInstance>(Spec->SourceObject.Get());
	}
	return nullptr;
}

// Good
// ItemDef -> ItemInstance -> EquipmentInstance -> Ability
ULyraInventoryItemInstance* ULyraGameplayAbility_FromEquipment::GetAssociatedItem() const
{
	if (ULyraEquipmentInstance* Equipment = GetAssociatedEquipment())
	{
		// In Lyra, equipment is equipped by inventory item instance:
		// - so, equipment's instigator should be inventory item instance
		// - otherwise, it will return nullptr by failing casting to LyraInventoryItemInstance
		return Cast<ULyraInventoryItemInstance>(Equipment->GetInstigator());
	}
	return nullptr;
}
