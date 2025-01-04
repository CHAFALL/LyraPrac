// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "LyraCosmeticAnimationTypes.generated.h"

USTRUCT(BlueprintType)
struct FLyraAnimLayerSelectionEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> Layer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer RequiredTags;
};

USTRUCT(BlueprintType)
struct FLyraAnimLayerSelectionSet
{
	GENERATED_BODY()

	/** CosmeticTags 기반하여, 적절한 AnimLayer를 반환한다 */
	TSubclassOf<UAnimInstance> SelectBestLayer(const FGameplayTagContainer& CosmeticTags) const;

	/** 앞서 보았던 LyraAnimBodyStyleSelection의 MeshRule과 같이 AnimInstance의 Rule을 가진 LayerRules로 생각하면 됨 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLyraAnimLayerSelectionEntry> LayerRules;

	/** 디폴트 Layer */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> DefaultLayer;
};


// ex. 전사 - 인간의 태그를 넣어둠.
USTRUCT(BlueprintType)
struct FLyraAnimBodyStyleSelectionEntry
{
	GENERATED_BODY()

	/** AnimLayer를 적용할 대상 SkeletalMesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh = nullptr;

	/** Cosmetic Tag라고 생각하면 됨 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Cosmetic"))
	FGameplayTagContainer RequiredTags;
};


USTRUCT(BlueprintType)
struct FLyraAnimBodyStyleSelectionSet
{
	GENERATED_BODY()

	/** GameplayTag를 통해 (CosmeticTags), Mesh Rules에 따라 알맞은 BodyStyle를 번환한다 */
	USkeletalMesh* SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const;

	/** AnimLayer 적용할 SkeletalMesh를 들고 있음 -> Animation-Mesh간 Rules을 MeshRules라고 생각하면 됨 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLyraAnimBodyStyleSelectionEntry> MeshRules;

	// 올바른 메시가 없을 시
	/** 그냥 디폴트로 적용할 SkeletalMesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> DefaultMesh = nullptr;

	/** Physics Asset은 하나로 동일함 -> 즉 모든 Animation의 Physics 속성은 공유함 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPhysicsAsset> ForcedPhysicsAsset = nullptr;
};


// 애니메이션을 사용하기 위해서는 메시(Mesh)가 장착이 되어야 됨.
// 전사는 인간 괴물 동물 중에 인간이겠지? 따라서 인간 메시를 장착해줘야 겠지 이런 것들을 정하는 MeshRules

// 이렇게 복잡하게 한 이유
// 기존에는 애니메이션 레이어를 하나만 구성해서 그거에다가 많은 불리언 값을 설정했었음 (이건 칼이고 저건 총이야..)
// 다만, 이런 패턴을 이용한다면 총이면 총, 칼이면 칼 딱 맞는 애니메이션 레이어를 관리 가능. (이렇게 하려면 현재 장착한 거에 맞는 스켈레탈 메시가 필요!)
// 그런 것들을 관리하기 위해서 태크, 메시를 관리함. (HTML, CSS 느낌)

// HTML이 FLyraAnimBodyStyleSelectionSet이고 CSS는 FLyraAnimBodyStyleSelectionEntry 느낌.