// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LyraInventoryItemInstance.generated.h"

class ULyraInventoryItemFragment;
class ULyraInventoryItemDefinition;

/**
 * 
 */
UCLASS(Blueprintable)
class LYRAPRAC_API ULyraInventoryItemInstance : public UObject
{
	GENERATED_BODY()
public:
	ULyraInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintPure = false, meta = (DeterminesOutputType = FragmentClass))
	const ULyraInventoryItemFragment* FindFragmentByClass(TSubclassOf<ULyraInventoryItemFragment> FragmentClass) const;

	// 얘의 역할은 위의 함수(FindFragmentByClass)를 불러주는 것임
	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}

	/** Inventory Item의 인스턴스에는 무엇으로 정의되었는지 메타 클래스인 LyraInventoryItemDefinition을 들고 있다 */
	UPROPERTY()
	TSubclassOf<ULyraInventoryItemDefinition> ItemDef;
};

// 보면 얘를 통해 ULyraInventoryItemDefinition를 캐싱했을 뿐만 아니라
// Fragment도 가져올 수 있음을 알 수 있음.


// Good
// Definition을 만들어서 확장한 것을 Instance
// Definition :  메타 데이터를 들고있음 (설계도 느낌.)
// Instance : 그 메타 데이터를 이용하여 생성해 관리

// Definition을 인벤토리에다가 저장하게 되면 아이템 인스턴스라는 단위로 저장을 하게 된다.

// 복습 (Cosmetic)
// CharacterPart라는 메타 데이터가 있었고 적용할 때는 AppliedCharacterPartEntry를 이용했었음.
