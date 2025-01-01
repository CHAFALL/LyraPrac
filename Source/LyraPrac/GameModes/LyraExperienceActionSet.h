// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraExperienceActionSet.generated.h"

class UGameFeatureAction;

/**
 * 
 */
UCLASS()
class LYRAPRAC_API ULyraExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	ULyraExperienceActionSet();

	/**
	 * member variables
	 */
	UPROPERTY(EditAnywhere, Category = "Actions to Perform")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};


// Action을 모아두는 Array
// -> 왜 굳이 2번 랩핑? 카테고리라는 시스템을 만들고 싶어서!
// GameFeature가 Action(행동의 단위)을 기준으로 뭔가를 수행하는데 FPS, AOS이런 각각의 모드에 따라서 액션이 다를 것인데 그런 것들을 모아놓기 위한 랩핑 클래스

// 이건 누가 들고 있어야 할까?
// -> LyraExperienceDefinition.


// B_Hero_ShooterMannequin
// input이 안 받아지잖아?
// -> 상속받고 있는 B_Hero Default에서 Default Input Config를 설정 안해줬잖아! (PMI) (맵핑 컨텍스트가 없어서 액션이 잘 바인딩이 되어도 어떤 키를 눌러야 바인딩이 될지가 결정이 안됨...)
// -> 그럼 해주면 되겠네? No. 이건 하드코딩에 가까운 행동!, 여러모드가 있을텐데 다 넣어두면 분기 처리를 해줘야 된다는거잖아.. -> 그럼 종속성이 또 생기겠지?
// -> 그럼 어떻게 할껀데? gameFeature의 Action (gameFeature가 활성화 되었을 때 어떠한 동작을 수행하는 단위)를 이용해서
// 동적으로 맵핑 컨텍스트를 설정해줄 것이다!

// 위의 내용 GPT 정리
// 1. 현재 문제점: 
// B_Hero_ShooterMannequin이 input을 받지 못하는 상황
// 상위 클래스(B_Hero Default)에서 Default Input Config가 설정되지 않음
// PMI(Player Mapping Input) 컨텍스트가 없어서 키 바인딩이 제대로 작동하지 않음
// 2. 간단한 해결책의 문제점:
// Default Input Config를 하드코딩하는 것은 좋지 않은 접근
// 여러 게임 모드에 대한 분기 처리가 필요해짐
// 불필요한 종속성 발생 가능성
// 3. 제안된 해결책:
// GameFeature의 Action을 활용
// 동적으로 매핑 컨텍스트를 설정
// 모듈화된 방식으로 입력 처리 구현


