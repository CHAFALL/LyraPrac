// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraCharacterPartTypes.h"
#include "LyraCosmeticAnimationTypes.h"
#include "Components/PawnComponent.h"
#include "LyraPawnComponent_CharacterParts.generated.h"

class ULyraPawnComponent_CharacterParts;

/** 인스턴스화 된 Character Part의 단위 */
USTRUCT()
struct FLyraAppliedCharacterPartEntry
{
	GENERATED_BODY()

	/** Character Part의 정의(메타 데이터) */
	UPROPERTY()
	FLyraCharacterPart Part;

	/** LyraCharacterPartList에서 할당 받은 Part 핸들값 (FLyraControllerCharacterPartEntry의 Handle 값과 같아야 함 -> 같으면 같은 Part) */
	UPROPERTY()
	int32 PartHandle = INDEX_NONE;

	// 스폰 정보. - 이게 필요한 이유? 메시를 바꿀 때 기존의 메시 제거가 되어야 되므로.
	/** 인스턴스화 된 Character Part용 Actor */
	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;

	// Actor에 직접 다른 Actor를 자식으로 붙일 수 x -> UChildActorComponent를 사용해야 됨. 
};


/** LyraPawnComponent_CharacterParts에서 실질적 Character Parts를 관리하는 클래스 */
USTRUCT(BlueprintType)
struct FLyraCharacterPartList
{
	GENERATED_BODY()

	FLyraCharacterPartList()
		: OwnerComponent(nullptr)
	{}

	FLyraCharacterPartList(ULyraPawnComponent_CharacterParts* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{}

	bool SpawnActorForEntry(FLyraAppliedCharacterPartEntry& Entry);
	void DestroyActorForEntry(FLyraAppliedCharacterPartEntry& Entry);

	FLyraCharacterPartHandle AddEntry(FLyraCharacterPart NewPart);
	void RemoveEntry(FLyraCharacterPartHandle Handle);

	FGameplayTagContainer CollectCombinedTags() const;

	/** 현재 인스턴스화된 Character Part */
	UPROPERTY()
	TArray<FLyraAppliedCharacterPartEntry> Entries;

	/** 해당 LyraCharacterPartList의 Owner인 PawnComponent!!!! */
	UPROPERTY()
	TObjectPtr<ULyraPawnComponent_CharacterParts> OwnerComponent;

	/** 앞서 보았던 PartHandle의 값을 할당 및 관리하는 변수 */
	int32 PartHandleCounter = 0;
};

/**
 * 
 */
UCLASS()
class LYRAPRAC_API ULyraPawnComponent_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()
	
public:
	ULyraPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	USkeletalMeshComponent* GetParentMeshComponent() const;
	USceneComponent* GetSceneComponentToAttachTo() const;

	// 현재 내가 장착되어 있는 Cosmetic에 관련된 Tag들을 가져오는 함수
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Cosmetics)
	FGameplayTagContainer GetCombinedTags(FGameplayTag RequiredPrefix) const;
	void BroadcastChanged();

	FLyraCharacterPartHandle AddCharacterPart(const FLyraCharacterPart& NewPart);
	void RemoveCharacterPart(FLyraCharacterPartHandle Handle);

	/** 인스턴스화 된 Character Parts */
	UPROPERTY()
	FLyraCharacterPartList CharacterPartList;

	/** 애니메이션 적용을 위한 메시와 연결고리 */
	UPROPERTY(EditAnywhere, Category = Cosmetics)
	FLyraAnimBodyStyleSelectionSet BodyMeshes;
};


// 파츠 리스트, 파츠 엔트리 이런 식으로 관리를 하고 있는데 왜 굳이 이렇게?
// Lyra는 데디 서버를 고려하고 만들다보니 네트워크 고려 (데이터를 적게 보내기 위해)

// 언리얼 네트워크 방식 -> 배열 하나만 수정이 되어도 전체를 다 보냄.... (replicated..)
// [][][][][][][][][][]
// -> 이를 방지하기 위해서 Network Serialized Array라는 고급 문법을 이용. (수정한 부분만 보냄.)
// 대신에 해당 기능을 사용하기 위해서는 파츠 리스트, 파츠 엔트리처럼 2개로 나눠야 된다.
// 하나는 Array 역할을 하는 Serialized이고 하나는 배열 안에 있는 구성 요소를 담당.


// (참고)
// FPartsList(Array 역할을 하는 Serialized) :
// 전체 배열 구조를 관리
// 배열의 구조적인 부분을 담당(크기, 순서 등)
// 
// FPartsEntry(배열 안의 구성 요소) :
// 실제 각 항목의 데이터를 담고 있음
// 배열 내의 개별 요소의 내용을 담당




