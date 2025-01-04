#pragma once

#include "LyraInventoryItemDefinition.h"
#include "Templates/SubclassOf.h"
#include "LyraInventoryFragment_EquippableItem.generated.h"

/** forward declaration */
class ULyraEquipmentDefinition;

UCLASS()
class ULyraInventoryFragment_EquippableItem : public ULyraInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Lyra)
	TSubclassOf<ULyraEquipmentDefinition> EquipmentDefinition;
};


// 중간 다리 역할

// ID_Pistol에 Fragment를 부여할 것인데 그게 ULyraInventoryFragment_EquippableItem라는 애이고 얘가 내부적으로 ULyraEquipmentDefinition를 들고 있으니까
// ID_Pistol가 WID_Pistol을 들고 있을 수 있다.

/**
* // 이런 식의 기능들을 Fragment에서 관리할 수 있습니다
- 장착 가능한 슬롯 정의
- 장착 레벨 제한
- 장착 직업/클래스 제한
- 장착 시 필요한 특수 조건들
*/