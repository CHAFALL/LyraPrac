// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonUserWidget.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "LyraWeaponUserInterface.generated.h"

/** forward declaration */
class ULyraWeaponInstance;

/**
 * 
 */
UCLASS()
class LYRAPRAC_API ULyraWeaponUserInterface : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	ULyraWeaponUserInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Weapon 변겅에 따른 BP Event */
	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponChanged(ULyraWeaponInstance* OldWeapon, ULyraWeaponInstance* NewWeapon);

	/**
	 * UUserWidget's interface
	 */
	// 비효율적인데??
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** 현재 장착된 WeaponInstance를 추적한다 (NativeTick을 활용하여 주기적 업데이트한다) */
	UPROPERTY(Transient)
	TObjectPtr<ULyraWeaponInstance> CurrentInstance;
};

