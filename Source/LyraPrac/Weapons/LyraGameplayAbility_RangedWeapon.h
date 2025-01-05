// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraPrac/Equipment/LyraGameplayAbility_FromEquipment.h"
#include "LyraGameplayAbility_RangedWeapon.generated.h"

class ULyraRangedWeaponInstance;

// 특히 3인칭 게임이면 카메라 위치가 위에서 대각선으로 보여줄 것인데
// 총구를 기준으로 맞춘걸로 한다고 하면
// 괴리감이 심해서 맞추기 매우 어려움.
// 그래서 카메라 기준으로 ㄲ

// 그냥 카메라 기준으로 하면 또 어떤 문제점이 있을까?
// 카메라 기준으로 하면 맞는 것인데 (카메라랑 매우 가까이 Target이 있을 시) 총구로 봤을 때
// 맞으면 안되는 것도 맞았다고 할 수 있음.

// 그래서 가장 많이 하는 방식이 카메라를 기준으로 하되 총구 지점을 기준으로 거리에 따라 맞은 여부를 판별

// 근접무기는 또 다른 기준으로 해야겠지???

// 어느 시점을 이용해서 Target 계산?
/** 어디 대상으로 Taget으로 설정할지 옵션들 (Lyra의 경우, 다양한 옵션 존재) */
UENUM(BlueprintType)
enum class ELyraAbilityTargetingSource : uint8
{
	/** Camera 기준으로 진행 */
	CameraTowardsFocus,
};

/**
 * 
 */
UCLASS()
class LYRAPRAC_API ULyraGameplayAbility_RangedWeapon : public ULyraGameplayAbility_FromEquipment
{
	GENERATED_BODY()
public:
	struct FRangedWeaponFiringInput
	{
		FVector StartTrace; // 시작지점
		FVector EndAim; //  끝나는 시점
		FVector AimDir; // 방향
		ULyraRangedWeaponInstance* WeaponData = nullptr;
		bool bCanPlayBulletFX = false;

		FRangedWeaponFiringInput()
			: StartTrace(ForceInitToZero)
			, EndAim(ForceInitToZero)
			, AimDir(ForceInitToZero)
		{}
	};

	ULyraGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	void StartRangedWeaponTargeting();

	void PerformLocalTargeting(TArray<FHitResult>& OutHits);
	FTransform GetTargetingTransform(APawn* SourcePawn, ELyraAbilityTargetingSource Source);
	FVector GetWeaponTargetingSourceLocation() const;
	void TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, TArray<FHitResult>& OutHits);
	FHitResult DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits) const;
	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHitResults) const;
	void AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParams) const;
	ECollisionChannel DetermineTraceChannel(FCollisionQueryParams& TraceParams, bool bIsSimulated) const;
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);

	/** called when target data is ready */
	UFUNCTION(BlueprintImplementableEvent)
	void OnRangeWeaponTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);

	ULyraRangedWeaponInstance* GetWeaponInstance();

};
