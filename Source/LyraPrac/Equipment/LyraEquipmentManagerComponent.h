// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "LyraEquipmentManagerComponent.generated.h"

/** forward declarations */
class ULyraEquipmentDefinition;
class ULyraEquipmentInstance;

USTRUCT(BlueprintType)
struct FLyraAppliedEquipmentEntry
{
	GENERATED_BODY()

	/** 장착물에 대한 메타 데이터 */
	UPROPERTY()
	TSubclassOf<ULyraEquipmentDefinition> EquipmentDefinition;

	/** EquipmentDefinition을 통해 생성도니 인스턴스 */
	UPROPERTY()
	TObjectPtr<ULyraEquipmentInstance> Instance = nullptr;
};

/**
 * 참고로 EquipmentInstance의 인스턴스를 Entry에서 관리하고 있다:
 * - LyraEquipmentList는 생성된 객체를 관리한다고 보면 된다
 */
USTRUCT(BlueprintType)
struct FLyraEquipmentList
{
	GENERATED_BODY()

	FLyraEquipmentList(UActorComponent* InOwnerComponent = nullptr)
		: OwnerComponent(InOwnerComponent)
	{}

	ULyraEquipmentInstance* AddEntry(TSubclassOf<ULyraEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(ULyraEquipmentInstance* Instance);

	/** 장착물에 대한 관리 리스트 */
	UPROPERTY()
	TArray<FLyraAppliedEquipmentEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};


/**
 * Pawn의 Component로서 장착물에 대한 관리를 담당한다
 */
UCLASS()
class LYRAPRAC_API ULyraEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()
	
public:
	ULyraEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ULyraEquipmentInstance* EquipItem(TSubclassOf<ULyraEquipmentDefinition> EquipmentDefinition);
	void UnequipItem(ULyraEquipmentInstance* ItemInstance);

	UPROPERTY()
	FLyraEquipmentList EquipmentList;
};
