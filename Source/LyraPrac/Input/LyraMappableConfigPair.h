// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LyraMappableConfigPair.generated.h"

/** forward declaration */
class UPlayerMappableInputConfig;

USTRUCT()
struct FLyraMappableConfigPair
{
	GENERATED_BODY()
public:
	FLyraMappableConfigPair() = default;

	// 보면 TSoftObjectPtr인 이유는 서버에선 필요없기 때문임을 알 수 있다.
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> Config;

	UPROPERTY(EditAnywhere)
	bool bShouldActivateAutomatically = true;
};

// Input Mapping Context 설정을 관리하기 위한 데이터 구조