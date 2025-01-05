// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraAnimInstance.h"
#include "AbilitySystemGlobals.h"

// 애니메이션 인스턴스가 최초로 초기화되는 시점이어서
// ASC가 등록이 안된 상태에서 InitializeWithAbilitySystem()가 호출되는 경우가 있어서
// 또 따로 처리를 해줘야 됨. -> AbilitySystemComponent의 InitAbilityActorInfo에서
void ULyraAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(ASC);
		}
	}
}

void ULyraAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	// ASC 내부 관리하는 GameplayTag와 AnimInstance의 멤버 Property와 Delegate를 연결하여, 값 변화에 대한 반영을 진행한다
	GameplayTagPropertyMap.Initialize(this, ASC);
}
