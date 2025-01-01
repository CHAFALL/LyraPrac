// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "LyraPrac/Input/LyraMappableConfigPair.h"
#include "LyraHeroComponent.generated.h"

struct FInputActionValue;
class ULyraCameraMode;

/**
 * component that sets up input and camera handling for player controlled pawns (or bots that simulate players)
 * - this depends on a PawnExtensionComponent to coordinate initialization
 *
 * 카메라, 입력 등 플레이어가 제어하는 시스템의 초기화를 처리하는 컴포넌트
 */

// 기존에는 캐릭터 자체에 종속적으로 인풋 같은 것을 설정했었음
// 이제는 그렇게 하지 않겠다 (why? 캐릭터에서 인풋에 대한 종속성이 생기므로)

// 
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class LYRAPRAC_API ULyraHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
public:
	ULyraHeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** FeatureName 정의 */
	static const FName NAME_ActorFeatureName;

	// (추가)
	/** Extension Event 이름 정의 */
	static const FName NAME_BindInputsNow;

	/**
	 * UPawnComponent interface
	 */
	virtual void OnRegister() final;
	virtual void BeginPlay() final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;

	/**
	* IGameFrameworkInitStateInterface
	*/
	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	// 데이터가 변경이 되었을 때 핸들링하는 부분이 추가로 있음.
	// 시스템을 처리하는 컴포넌트이기 때문.
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) final;
	virtual void CheckDefaultInitialization() final;

	/**
	 * member methods
	 */
	TSubclassOf<ULyraCameraMode> DetermineCameraMode() const;
	void InitializePlayerInput(UInputComponent* PlayerInputComponent);
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);

	/**
	* member variables
	*/
	UPROPERTY(EditAnywhere)
	TArray<FLyraMappableConfigPair> DefaultInputConfigs;
};


// 핵심은
// 강제 업데이트랑
// OnActorInitStateChanged



// Cosmetic 시스템
// GameFeature 시스템의 근본은 컴포넌트!
// -> 컴포넌트를 붙이는 방식으로 캐릭터 메쉬를 붙여버림!!