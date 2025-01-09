// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "LyraHealthSet.generated.h"

/**
 * 
 */
UCLASS()
class LYRAPRAC_API ULyraHealthSet : public ULyraAttributeSet
{
	GENERATED_BODY()
	
public:
	ULyraHealthSet();

	/**
	* 앞서 LyraAttributeSet에서 정의했던, ATTRIBUTE_ACCESSORS를 통해, 아래 정의함 멤벼변수와 똑같이 이름을 설정한다
	* - ATTRIBUTE_ACCESSORS의 Macro의 정의부분을 한번 실펴보자
	*/
	// Getter, Setter, Init 같은거 쓰려면 아래와 같이 또 해줘야 됨.
	ATTRIBUTE_ACCESSORS(ULyraHealthSet, Health);
	ATTRIBUTE_ACCESSORS(ULyraHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(ULyraHealthSet, Healing);

	/**
	* Attribute의 값을 ClampAttribute()를 활용하여, 값의 범위를 유지시켜주기 위해
	* PreAttributeBaseChange와 PreAttributeChange 오버라이드
	*/
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
	// 변경 전에 이 함수를 거쳐서 최종 점검을 거침.
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/**
	* GameplayEffect가 HealthSet의 Attribute를 수정하기 전에 불리는 콜백함수이다:
	* - 이는 AttributeSet의 주석에도 잘 나와있듯이, Healing이 업데이트되면, Health를 Healing을 적용하여 업데이트 가능하다
	*/
	// 힐링 적용 관련
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/** 현재 체력 */
	UPROPERTY(BlueprintReadOnly, Category = "Lyra|Health")
	FGameplayAttributeData Health;

	/** 체력 최대치 */
	UPROPERTY(BlueprintReadOnly, Category = "Lyra|Health")
	FGameplayAttributeData MaxHealth;

	/** 체력 회복치 */
	UPROPERTY(BlueprintReadOnly, Category = "Lyra|Health")
	FGameplayAttributeData Healing;
};

// FGameplayAttributeData : 버프 여부를 구분하기 위해서 BaseValue와 CurrentValue를 분리해서 관리하기 위함.