// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraPawnData.generated.h"

class ULyraAbilitySet;
class ULyraInputConfig;
class ULyraCameraMode;

/**
 * 
 */
UCLASS()
class LYRAPRAC_API ULyraPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	
public:
	ULyraPawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// 3총사 + a

	// PawnData에서 어떤 폰을 스폰할 것인지 결정해줘야 됨.
	/** Pawn의 Class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Pawn")
	TSubclassOf<APawn> PawnClass;

	/** Camera Mode */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Camera")
	TSubclassOf<ULyraCameraMode> DefaultCameraMode;

	/** input configuration used by player controlled pawns to create input mappings and bind input actions */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|InputConfig")
	TObjectPtr<ULyraInputConfig> InputConfig;

	// 이 AbilitySets 설정은 PlayerState에서 진행됨
	/** 해당 Pawn의 Ability System에 허용할 AbilitySet */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Abilities")
	TArray<TObjectPtr<ULyraAbilitySet>> AbilitySets;
};


// cf
//PawnClass:
//GameMode나 GameState에서 스폰할 때 참조
//Character 스폰 시점에 사용됨
//
//
//DefaultCameraMode :
//LyraCameraComponent에서 설정
//ULyraHeroComponent::DetermineCameraMode에서 실제로 적용
//
//
//InputConfig :
//ULyraHeroComponent::InitializePlayerInput에서 설정
//Input Mappings와 Actions를 바인딩할 때 사용
//
//
//AbilitySets :
//PlayerState에서 설정(주석에 명시된 대로)
//ULyraAbilitySystemComponent::GiveAbilitySets를 통해 실제 적용


// FObjectInitializer가 중요한 이유:
// 1.언리얼 엔진의 리플렉션 시스템과 함께 작동합니다
// 2.객체 생성 시 필요한 기본 설정을 처리합니다
// 3.CDO(Class Default Object) 생성을 관리합니다
// 4.프로퍼티 초기화를 담당합니다
// 주로 UCLASS를 상속받은 새로운 클래스를 만들 때 해줘야 됨.