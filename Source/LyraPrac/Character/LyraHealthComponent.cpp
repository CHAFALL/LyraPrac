// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraHealthComponent.h"
#include "LyraPrac/LyraLogChannels.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "LyraPrac/AbilitySystem/LyraAbilitySystemComponent.h"
#include "LyraPrac/AbilitySystem/Attributes/LyraHealthSet.h"

ULyraHealthComponent::ULyraHealthComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// HealthComponent는 PlayerState의 HealthSet과 Character(Pawn)간 Bridge 역활이라고 생각하면 된다:
	// - 따로 로직이 업데이트 될 필요가 없는 이벤트 기반으로 동작하는 컴포넌트로 이해하면 된다
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	// InitializeWithAbilitySystem으로 ASC가 초기화되기 전까지 HealthSet과 ASC는 null이다:
	AbilitySystemComponent = nullptr;
	HealthSet = nullptr;
}

// InitializeWithAbilitySystem에 의해서 ASC가 등록이 되어야 되는데
// 어느타이밍에 해야 좋을까??? (4가지 조건이 충족되어야 됨)
// 1.어빌리티 시스템이 완벽히 초기화 되어야 됨
// 2. PlayerState 세티잉 잘 되어야 됨
// 3. Pawn에도 잘 등록이 되어야 됨. (Pawn에 등록이 되어야지 Possess가 되었을테니깐 캐릭터가 정상적으로 등록이 되었겠지?)
// 4. Experience 관련 모든 세팅이 다 적용이 되어야 됨.
// -> PawnExtensionComponent, HeroComponent으로 ㄲㄲ
void ULyraHealthComponent::InitializeWithAbilitySystem(ULyraAbilitySystemComponent* InASC)
{
	// AActor는 LyraCharacter를 상속받고 있는 클래스일 것이다
	AActor* Owner = GetOwner();
	check(Owner);

	if (AbilitySystemComponent)
	{
		UE_LOG(LogLyra, Error, TEXT("LyraHealthComponent: Health component for owner [%s] has already been initialized with an ability system."), *GetNameSafe(Owner));
		return;
	}

	// ASC 캐싱
	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogLyra, Error, TEXT("LyraHealthComponent: Cannot initialize health component for owner [%s] with NULL ability system."), *GetNameSafe(Owner));
		return;
	}

	// AbilitySystemComponent::GetSet은 SpawnedAttributes에서 가져온다:
	// - 근데 PlayerState에서 Subobject로 생성하고 따로 ASC에 등록한적이 없는데 어떻게 등록되어있을까?
	//   - AbilitySystemComponent::InitializeComponent()에서 GetObjectsWithOuter로 SpawnedAttributes에 추가된다:
	//   - 잘 생각해보자 HealthSet은 PlayerState에 Subobject로 있고, ASC 또한 PlayerState에 있다:
	//     -> 이는 ASC에서 GetObjectsWithOuter로 HealthSet이 접근된다!!!
	// - 한번 AbilitySystemComponent::InitializeComponent()을 보자
	HealthSet = AbilitySystemComponent->GetSet<ULyraHealthSet>();
	if (!HealthSet)
	{
		UE_LOG(LogLyra, Error, TEXT("LyraHealthComponent: Cannot initialize health component for owner [%s] with NULL health set on the ability system."), *GetNameSafe(Owner));
		return;
	}

	// HealthSet의 HealthAttribute의 업데이트가 일어날때마다 호출할 콜백으로 멤버메서드 HandleHealthChanged를 등록하자:
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ULyraHealthSet::GetHealthAttribute()).AddUObject(this, &ThisClass::HandleHealthChanged);

	// 초기화 한번 해줬으니깐 Broadcast 해주자
	OnHealthChanged.Broadcast(this, 0, HealthSet->GetHealth(), nullptr);
}

void ULyraHealthComponent::UninitializeWithAbilitySystem()
{
	AbilitySystemComponent = nullptr;
	HealthSet = nullptr;
}

// ChangeData 안에 누구에 의해서 체력이 바뀌었는지에 대한 정보를 가지고 있음.
static AActor* GetInstigatorFromAttrChangeData(const FOnAttributeChangeData& ChangeData)
{
	// GameEffectModifier에 Data가 있을 경우만 호출되는가보다 (사실 우리는 크게 관심없음)
	if (ChangeData.GEModData != nullptr)
	{
		const FGameplayEffectContextHandle& EffectContext = ChangeData.GEModData->EffectSpec.GetEffectContext();
		return EffectContext.GetOriginalInstigator();
	}
	return nullptr;
}

void ULyraHealthComponent::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	OnHealthChanged.Broadcast(this, ChangeData.OldValue, ChangeData.NewValue, GetInstigatorFromAttrChangeData(ChangeData));
}

ULyraHealthComponent* ULyraHealthComponent::FindHealthComponent(const AActor* Actor)
{
	if (!Actor)
	{
		return nullptr;
	}

	ULyraHealthComponent* HealthComponent = Actor->FindComponentByClass<ULyraHealthComponent>();
	return HealthComponent;
}

float ULyraHealthComponent::GetHealth() const
{
	return (HealthSet ? HealthSet->GetHealth() : 0.0f);
}

float ULyraHealthComponent::GetMaxHealth() const
{
	return (HealthSet ? HealthSet->GetMaxHealth() : 0.0f);
}

float ULyraHealthComponent::GetHealthNormalized() const
{
	if (HealthSet)
	{
		const float Health = HealthSet->GetHealth();
		const float MaxHealth = HealthSet->GetMaxHealth();
		return ((MaxHealth > 0.0f) ? (Health / MaxHealth) : 0.0f);
	}
	return 0.0f;
}