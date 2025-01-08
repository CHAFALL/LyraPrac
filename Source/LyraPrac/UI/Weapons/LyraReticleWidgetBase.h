#pragma once

#include "CommonUserWidget.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "LyraReticleWidgetBase.generated.h"

/** forward declarations */
class ULyraWeaponInstance;
class ULyraInventoryItemInstance;

UCLASS(Abstract)
class ULyraReticleWidgetBase : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	ULyraReticleWidgetBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	void InitializeFromWeapon(ULyraWeaponInstance* InWeapon);

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponInitialized();

	/**
	 * WeaponInstance/InventoryInstance를 상태 추적용으로 캐싱 목적
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULyraWeaponInstance> WeaponInstance;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULyraInventoryItemInstance> InventoryInstance; // 이걸 들고 있는 이유? Fragment를 가져오기 위함.
};