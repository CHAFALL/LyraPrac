#pragma once

#include "Containers/Array.h"
#include "Templates/SubclassOf.h"
#include "LyraPrac/Inventory/LyraInventoryItemDefinition.h"
#include "InventoryFragment_ReticleConfig.generated.h"

/** forward declaration */
class ULyraReticleWidgetBase;

UCLASS()
class ULyraInventoryFragment_ReticleConfig : public ULyraInventoryItemFragment
{
	GENERATED_BODY()
public:
	/** 무기에 결합된 ReticleWidget 정보를 가지고 있는 Fragment */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Reticle)
	TArray<TSubclassOf<ULyraReticleWidgetBase>> ReticleWidgets;
};

// 장착하는 무기에 따라서 조준선이 다르므로 이걸 만들었다고 보면 됨.