// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraCameraMode.h"
#include "Camera/CameraComponent.h"
#include "LyraCameraComponent.generated.h"

class ULyraCameraModeStack;

/** template forward declaration */
template <class TClass> class TSubclassOf;

// ULyraCameraMode가 반환되는 델리게이트 생성
/** (return type, delegate_name) */
DECLARE_DELEGATE_RetVal(TSubclassOf<ULyraCameraMode>, FLyraCameraModeDelegate);

UCLASS()
class LYRAPRAC_API ULyraCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
	
public:
	ULyraCameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static ULyraCameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<ULyraCameraComponent>() : nullptr); }


	/**
	* member methods
	*/
	AActor* GetTargetActor() const { return GetOwner(); }
	void UpdateCameraModes();

	/**
	 * CameraComponent interface
	 */
	virtual void OnRegister() final;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) final;

	/**
	 * member variables
	 */
	 /** 카메라의 blending 기능을 지원하는 stack */
	// 카메라가 시야를 조정해야 되므로
	UPROPERTY()
	TObjectPtr<ULyraCameraModeStack> CameraModeStack;

	/** 현재 CameraMode를 가져오는 Delegate */
	FLyraCameraModeDelegate DetermineCameraModeDelegate;
};




//PlayerCameraManager와 CameraComponent의 차이점
//
//- PlayerCameraManager
//
//- 실질적으로 보이는 시야를 결정
//
//- 게임의 전반적인 카메라 시스템을 관리하고 제어
//
//- CameraComponent
//
//- 특정 액터의 시점 정보를 제공하는 것이 주 목적
//- 캐릭터의 시야를 캡처하지만, 직접 렌더링하지 않음
//- 대신 캡처한 시점 정보를 PlayerCameraManager에 전달하여 최종 출력을 담당하게 함
//- CameraComponent가 직접 렌더링하지 않는 이유
//
//- 플레이어 컨트롤러는 게임 중 다른 캐릭터로 자유롭게 전환될 수 있음
//- 이러한 유연성을 지원하기 위해 카메라 시스템이 특정 캐릭터에 종속되지 않아야 함
//- 따라서 PlayerController 내부에 CameraManager를 두어 캐릭터 독립적인 카메라 시스템을 구현