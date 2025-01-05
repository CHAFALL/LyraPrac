// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraAbilitySystemComponent.h"
#include "Abilities/LyraGameplayAbility.h"
#include "GameFramework/Pawn.h"
#include "LyraPrac/Animation/LyraAnimInstance.h"

ULyraAbilitySystemComponent::ULyraAbilitySystemComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}


// 새로운 Pawn이 설정될 때마다 애니메이션 시스템을 어빌리티 시스템과 연동
void ULyraAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
	check(ActorInfo);
	check(InOwnerActor);

	const bool bHasNewPawnAvatar = Cast<APawn>(InAvatarActor) && (InAvatarActor != ActorInfo->AvatarActor);

	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (bHasNewPawnAvatar)
	{
		if (ULyraAnimInstance* LyraAnimInst = Cast<ULyraAnimInstance>(ActorInfo->GetAnimInstance()))
		{
			LyraAnimInst->InitializeWithAbilitySystem(this);
		}
	}
}

void ULyraAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		// 허용된 GameplayAbilitySpec을 순회
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			// Ability가 존재하고, DynamicAbilityTags에 InputTag에 있을 경우, InputPressed/Held에 넣어 Ability 처리를 대기한다
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void ULyraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				// Released에 추가하고, Held에서는 제거해준다
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

// 얘를 처리해줄 곳은? - tick에 할까하는데 그건 위험한 짓이다.
// 왜냐하면 ex. 점프(화살표 위)와 공격(스페이스바)을 했을 때 
// tick 시점이 스페이스바가 딱 끝나고 발동한다면 점프는 바로 발동했는데 공격은 뒤늦게 발동. (둘이 끊어짐...)
// 그럼 언제? 모든 인풋이 끝나고 모든 키가 눌리고 한번에 처리가 될 수 있는 곳에서 (PlayerController에 있음. PostProcessInput 부분.)
//
void ULyraAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;

	// InputHeldSpecHandles에 대해 Ability 처리를 위해 AbilitiesToActivate에 추가한다
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		// FindAbilitySpecFromHandle 확인:
		// - ActivatableAbilities의 Handle 값 비교를 통해 GameplayAbilitySpec을 반환한다
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const ULyraGameplayAbility* LyraAbilityCDO = CastChecked<ULyraGameplayAbility>(AbilitySpec->Ability);

				// ActivationPolicy가 WhileInputActive 속성이면 활성화로 등록
				if (LyraAbilityCDO->ActivationPolicy == ELyraAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					// 이미 Ability가 활성화되어 있을 경우, Input Event(InputPressed)만 호출
					// - AbilitySpecInputPressed 확인
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const ULyraGameplayAbility* LyraAbilityCDO = CastChecked<ULyraGameplayAbility>(AbilitySpec->Ability);

					// ActivationPolicy가 OnInputTriggered 속성이면 활성화로 등록
					if (LyraAbilityCDO->ActivationPolicy == ELyraAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	// 등록된 AbilitiesToActivate를 한꺼번에 등록 시작:
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		// 모든 것이 잘 진행되었다면, CallActivate 호출로 BP의 Activate 노드가 실행될 것임
		TryActivateAbility(AbilitySpecHandle);
	}

	// 이번 프레임에 Release되었다면, 관련 GameplayAbility 처리:
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;
				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	// InputHeldSpecHandles은 InputReleasedSpecHandles 추가될때 제거된다!
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

