// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LyraEquipmentDefinition.generated.h"

class ULyraAbilitySet;
class ULyraEquipmentInstance;

USTRUCT()
struct FLyraEquipmentActorToSpawn
{
	GENERATED_BODY()

	/** Spawn할 대상 Actor 클래스 (== Actor를 상속받은 Asset으로 생각해도 됨) */
	UPROPERTY(EditAnywhere, Category = Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	/** 어느 Bone Socket에 붙일지 결정한다 */
	UPROPERTY(EditAnywhere, Category = Equipment)
	FName AttachSocket;

	/** Socket에서 어느정도 Transformation을 더할것인지 결정: (Rotation, Position, Scale) */
	UPROPERTY(EditAnywhere, Category = Equipment)
	FTransform AttachTransform;
};

/**
 * LyraEquipementDefinition은 장착 아이템에 대한 정의 클래스(메타 데이터)이다
 */
UCLASS(Blueprintable)
class LYRAPRAC_API ULyraEquipmentDefinition : public UObject
{
	GENERATED_BODY()
	
public:
	ULyraEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** 해당 메타 데이터를 사용하면, 어떤 인스턴스를 Spawn할지 결정하는 클래스 */
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TSubclassOf<ULyraEquipmentInstance> InstanceType;

	/** 해당 장착 아이템을 사용하면, 어떤 Actor가 Spawn이 되는지 정보를 담고 있다 */
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<FLyraEquipmentActorToSpawn> ActorsToSpawn;

	// 무기에 따라 능력이 다를테니
	/** 장착을 통해 부여 가능한 Ability Set */
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<TObjectPtr<ULyraAbilitySet>> AbilitySetsToGrant;
	
};

// 원래 인스턴스 -  definition(대표) 파일을 소유하게 되면서 클래스로 생성해서 관리하는 형태
// 이번 인스턴스 - 소환이 되고나서 관리 방식을 결정하는 것 (생성된 주체는 맞는데 관리 대상자, 관리자 느낌으로 생각하면 될 듯.)

// 요약.
// "원래 인스턴스": Definition을 소유하고 있는 클래스 기반의 관리 형태
// "이번 인스턴스": 스폰된 후의 실제 관리자 / 컨트롤러 역할
// 즉, 생성된 객체들을 실제로 관리하고 제어하는 역할을 담당