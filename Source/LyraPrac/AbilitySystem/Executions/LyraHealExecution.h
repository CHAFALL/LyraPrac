// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "LyraHealExecution.generated.h"

/**
 * 
 */
UCLASS()
class LYRAPRAC_API ULyraHealExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	ULyraHealExecution();

	// 우리만의 힐 계산 공식을 만들어놓을 것임
	// ex. 힐량 * 2배
	/** 해당 메서드는 GameplayEffectExecutionCalculation의 Execute() BlueprintNativeEvent를 오버라이드 한다 */
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};


// _Implementation가 붙는 것 특징
// -> (BlueprintNativeEvent) 라고 되어있는데 C++에서 구현할꺼면 담.