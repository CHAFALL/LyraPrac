// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "LyraAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class LYRAPRAC_API ULyraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	ULyraAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * AbilitySystemComponent's interface
	 */
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	/**
	 * member methods
	 */
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

	/** Ability Input 처리할 Pending Queue */
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};

// 태그랑 어빌리티가 굉장히 많아질텐데 어떤 어빌리티마다 계속 함수를 바인딩하긴 좀...
// -> 그래서 Lyra에서는 ULyraHeroComponent의 Input_AbilityInputTagPressed, Input_AbilityInputTagReleased 함수를 통해
// 키를 누를 때마다 그 키에 맞는 태그를 넘기고 그 태그를 InputPressedSpecHandles, InputReleasedSpecHandles, InputHeldSpecHandles에다가 집어넣고
// ProcessAbilityInput에서 처리를 해줌. (이걸 매 프레임마다 해줌)
// -> 장점 : 하드코딩 x, 태그로만 연결이 되어있기 때문에 같은 입력이라도 무기나 상황에 따라 다른 어빌리티 발동 가능

// cf. 작동 과정
// 플레이어가 마우스 왼쪽 버튼 클릭
// "Input.Combat.Fire" 태그가 전달됨
// 현재 장착된 무기의 어빌리티 셋에서 이 태그를 가진 어빌리티를 찾음
// 찾은 어빌리티 실행



// 여기에 부여되는 능력들만 플레이어가 사용이 가능
// Ability System의 매니저 느낌.

// 이건 어디에 붙이는 것이 맞을까? (총 2가지 가능)
// PlayerState에 달기 - 이걸로 하자!! 
// Character에 달기 (싱글 + 플레이어가 고정된 경우에만)

// PlayerState 존재 이유
// -> 멀티플레이어에서 PlayerController는 서버 측에서 각 플레이어를 대표
// PlayerController의 직접적인 공유는 보안상 위험
// PlayerState는 다른 플레이어들과 공유해야 하는 정보를 안전하게 전달하는 메커니즘 제공

// GameState와 PlayerState의 존재 이유
// GameState: GameMode의 정보를 클라이언트에 안전하게 전달하는 복사본 역할
// PlayerState: PlayerController의 공유 가능한 정보를 담는 컨테이너
// 보안 이슈로 
// 서버의 핵심 컴포넌트(GameMode, PlayerController)는 직접 공유하지 않음
// 대신 State 객체들(GameState, PlayerState)을 통해 필요한 정보만 전달

// 어빌리티 시스템은 모든 애들을 다 알아야 겠지? - 그래야 반영을 할테니까
// 그래서 PlayerState에 장착 시킴.
