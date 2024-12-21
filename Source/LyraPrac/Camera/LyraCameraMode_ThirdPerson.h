// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraCameraMode.h"
#include "LyraCameraMode_ThirdPerson.generated.h"

class UCurveVector; // 그래프로 되어있는 벡터형태

/**
 * 
 */
// Abstract : 카메라모드를 한번이라도 상속받아야만 가능 (이 자체적으로는 사용 불가하도록 만든 키워드)
UCLASS(Abstract, Blueprintable)
class LYRAPRAC_API ULyraCameraMode_ThirdPerson : public ULyraCameraMode
{
	GENERATED_BODY()
public:
	ULyraCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	* ULyraCameraMode's interface
	*/
	virtual void UpdateView(float DeltaTime) override;

	/**
	 * member variables
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	TObjectPtr<const UCurveVector> TargetOffsetCurve; 

};
