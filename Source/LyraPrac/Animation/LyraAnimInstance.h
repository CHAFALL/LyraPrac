// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LyraAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LYRAPRAC_API ULyraAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	/** 해당 속성값은 Lyra의 AnimBP에서 사용되는 값이므로 정의해주자 */
	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.0f;
};

// 이게 전부? 
// Lyra는 애니메이션 관련된 것을 내부적으로 모든 변수와 로직을 전부 다 블루프린트에서 관리
// 정말 많은 것들이 묶여있음...

// CoreRedirect 이용. - 이를 사용하지 않았다면, Lyra에서 가져온 애셋들이 여전히 원본 ULyraAnimInstance 경로를
// 찾으려고 해서 참조가 깨짐... (Migration)