// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"
#include "Delegates/Delegate.h"
#include "LyraHealthComponent.generated.h"

/** forward declarations */
class ULyraAbilitySystemComponent;
class ULyraHealthSet;
class ULyraHealthComponent;
class AActor;
struct FOnAttributeChangeData;

/** Health 변화 콜백을 위한 델레게이트 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FLyraHealth_AttributeChanged, ULyraHealthComponent*, HealthComponent, float, OldValue, float, NewValue, AActor*, Instigator);

/**
 * Character(Pawn)에 대해 체력관련 처리를 담당하는 Component이다
 * - 참고로 해당 클래스는 Blueprintable이다:
 * - 이는 멤버변수인 Delegate를 UI에서 바인딩하기 위함이다 (자세한건 클론하면서 알아보자)
 */
UCLASS()
class LYRAPRAC_API ULyraHealthComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()
public:
	ULyraHealthComponent(const FObjectInitializer& ObjectInitializer);

	/** Actor(보통 ACharacter/APawn)의 HealthComponent를 반환 */
	UFUNCTION(BlueprintPure, Category = "Lyra|Health")
	static ULyraHealthComponent* FindHealthComponent(const AActor* Actor);

	/** 아래의 UFUNCTION은 HealthSet의 Attribute에 접근하기 위한 BP Accessor 함수들 */
	UFUNCTION(BlueprintCallable, Category = "Lyra|Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Lyra|Health")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Lyra|Health")
	float GetHealthNormalized() const;

	/** ASC와 HealthSet 초기화 */
	void InitializeWithAbilitySystem(ULyraAbilitySystemComponent* InASC);
	void UninitializeWithAbilitySystem();

	/** ASC를 통해, HealthSet의 HealthAttribute 변경이 있을때 호출하는 메서드 (내부적으로 OnHealthChanged 호출) */
	void HandleHealthChanged(const FOnAttributeChangeData& ChangeData);

	/** HealthSet을 접근하기 위한 AbilitySystemComponent */
	UPROPERTY()
	TObjectPtr<ULyraAbilitySystemComponent> AbilitySystemComponent;

	/** 캐싱된 HealthSet 레퍼런스 */
	UPROPERTY()
	TObjectPtr<const ULyraHealthSet> HealthSet;

	/** health 변화에 따른 Delegate(Multicast) */
	UPROPERTY(BlueprintAssignable)
	FLyraHealth_AttributeChanged OnHealthChanged;
	
};


// 주로 UI랑 캐릭터랑 서로 가리켜서 값을 수정했음
// -> 종속적이게 됨..., Tick을 이용...

// 이를 해결하기 위해
// UI와 Attribute 사이에 HealthComponent를 둬서 
// Attribute에서 값이 바뀌면 HealthComponent에 알려줌
// 그러면 HealthComponent가 UI에 알려줌 
// 서로 상황만 보고 있는 느낌.

// 이건 당연히 캐릭터 쪽에서 들고 있어야 겠지??