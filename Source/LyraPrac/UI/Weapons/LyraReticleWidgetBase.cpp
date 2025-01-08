#include "LyraReticleWidgetBase.h"
#include "LyraPrac/Weapons/LyraWeaponInstance.h"
#include "LyraPrac/Inventory/LyraInventoryItemInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraReticleWidgetBase)

ULyraReticleWidgetBase::ULyraReticleWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

void ULyraReticleWidgetBase::InitializeFromWeapon(ULyraWeaponInstance* InWeapon)
{
	WeaponInstance = InWeapon;
	InventoryInstance = nullptr;
	if (WeaponInstance)
	{
		InventoryInstance = Cast<ULyraInventoryItemInstance>(WeaponInstance->GetInstigator());
	}

	OnWeaponInitialized();
}