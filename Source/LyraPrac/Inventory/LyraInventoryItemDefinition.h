// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LyraInventoryItemDefinition.generated.h"

// 아이템을 상속 느낌이 아닌 컴포넌트 방식으로도 할 수 있구나..

/**
 * Inventory에 대한 Fragment은 확 와닫지 않을 수 있다:
 * - Lyra에서 사용하는 예시를 통해 이해해보자:
 *   - ULyraInventoryFragment_EquippableItem은 EquipmentItemDefinition을 가지고 있으며, 장착 가능한 아이템을 의미한다
 *   - ULyraInventoryFramgent_SetStats는 아이템에 대한 정보를 가지고 있다
 *     - Rifle에 대한 SetStats으로 총알(Ammo)에 대한 장착 최대치와 현재 남은 잔탄 수를 예시로 들 수 있다
 *	 - 등등...
 */
// ex. 권총 - [장착], 물약 - [소비], 1회용 권총 - [장착][소비] 이런 식으로 특성?을 장착하는 느낌 (컴포넌트 느낌에 가까움.)
// 스탯이라는 개념도 만들 수 있음.
// 핵심
UCLASS(Abstract, DefaultToInstanced, EditInlineNew)
class ULyraInventoryItemFragment : public UObject
{
	GENERATED_BODY()
public:
};


/**
 * 
 */
UCLASS(Blueprintable)
class LYRAPRAC_API ULyraInventoryItemDefinition : public UObject
{
	GENERATED_BODY()
	
public:
	ULyraInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const ULyraInventoryItemFragment* FindFragmentByClass(TSubclassOf<ULyraInventoryItemFragment> FragmentClass) const;


	/** Inventory Item 정의(메타) 이름 **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText DisplayName;

	/** Inventory Item의 Component를 Fragment로 인식하면 된다 */
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category = Display)
	TArray<TObjectPtr<ULyraInventoryItemFragment>> Fragments;
};


// 여기서 DefaultToInstanced, EditInlineNew, Instanced 이렇게 3개가 필수.
// 이 메타 키워드가 없으면 에디터 상에서 막 추가를 해도 
// 에디터 상에서만 적용이 된 것이기 때문에 CDO에도 반영이 안되고
// 결과적으로 게임이 실행될 때나 인스턴스가 생성될 때 설정했던 내용이 사라진다...

// DefaultToInstanced: Fragment들이 자동으로 인스턴스화됨
// EditInlineNew: 에디터에서의 수정사항이 실제 객체에 반영됨
// Instanced : 수정된 내용이 제대로 직렬화되어 저장됨

// Abstract : 이 자체로 사용 불가, 한번 상속 받은 이후부터 사용가능.
// CDO : 각 클래스당 하나씩 존재하는 기본 객체