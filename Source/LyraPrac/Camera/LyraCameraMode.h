// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LyraCameraMode.generated.h"

// 렌더링하고 싶은 즉, 바라봐야 될 화면에 나왔으면 좋겠는 그 영역을 저장.

class ULyraCameraComponent;

/**
 * FLyraCameraModeView
 */
struct FLyraCameraModeView
{
	FLyraCameraModeView();

	void Blend(const FLyraCameraModeView& Other, float OtherWeight);

	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};

/**
 * [0,1]을 BlendFunction에 맞게 재매핑을 위한 타입
 */
UENUM(BlueprintType)
enum class ELyraCameraModeBlendFunction : uint8
{
	Linear,
	/**
	 * EaseIn/Out은 exponent 값에 의해 조절된다:
	 */
	EaseIn,
	EaseOut,
	EaseInOut,
	COUNT
};

/** Camera Blending 대상 유닛 */
UCLASS(Abstract, NotBlueprintable)
class ULyraCameraMode : public UObject
{
	GENERATED_BODY()
public:
	ULyraCameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * member methods
	 */
	void UpdateCameraMode(float DeltaTime);
	virtual void UpdateView(float DeltaTime);
	void UpdateBlending(float DeltaTime);

	ULyraCameraComponent* GetLyraCameraComponent() const;
	AActor* GetTargetActor() const;
	// 기존의 스트링 암으로 캐릭터가 움직이면 카메라도 따라움직였던 것과 유사
	// 그걸 코드를 통해서 한 것임 그냥
	// Pivot : 카메라를 소유하고 있는 액터를 기준으로 결정이 됨.
	FVector GetPivotLocation() const;
	FRotator GetPivotRotation() const;

	/**
	 * member variables
	 */

	 /** CameraMode에 의해 생성된 CameraModeView */
	FLyraCameraModeView View;

	/** Camera Mode의 FOV */
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", Clampmax = "170.0"))
	float FieldOfView;

	// 목 꺾어서 보는 것 방지

	/** View에 대한 Pitch [Min, Max] */
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Clampmax = "89.9"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Clampmax = "89.9"))
	float ViewPitchMax;

	/** 얼마동안 Blend를 진행할지 시간을 의미 */
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	/** 선형적인 Blend 값 [0, 1] */
	float BlendAlpha;

	/**
	 * 해당 CameraMode의 최종 Blend 값
	 * 앞서 BlendAlpha의 선형 값을 매핑하여 최종 BlendWeight를 계산 (코드를 보며, 이해해보자)
	 */
	float BlendWeight;

	/**
	* EaseIn/Out에 사용한 Exponent
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;

	/** Blend function */
	ELyraCameraModeBlendFunction BlendFunction;
};

/** Camera Blending을 담당하는 객체 */
UCLASS()
class ULyraCameraModeStack : public UObject
{
	GENERATED_BODY()
public:
	ULyraCameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * member variables
	 */

	ULyraCameraMode* GetCameraModeInstance(TSubclassOf<ULyraCameraMode>& CameraModeClass);
	void PushCameraMode(TSubclassOf<ULyraCameraMode>& CameraModeClass);
	void EvaluateStack(float DeltaTime, FLyraCameraModeView& OutCameraModeView);
	void UpdateStack(float DeltaTime);
	void BlendStack(FLyraCameraModeView& OutCameraModeView) const;

	/**
	* member variables
	*/

	 /** 생성된 CameraMode를 관리 */
	// 카메라모드는 어느정도 정해져있을텐데 그걸 매번 NewObject로 매번 생성하는 것은
	// 비효율적이라고 판단해서 카메라모드 인스턴스를 생성할 때마다 캐싱을 해둠. (최적화)
	// 많지도 않아서 메모리 걱정은 안해도 됨.
	// 그리고 카메라모드는 속도도 빨라야 되자너
	UPROPERTY()
	TArray<TObjectPtr<ULyraCameraMode>> CameraModeInstances;

	/** Camera Matrix Blend 업데이트 진행 큐 */
	UPROPERTY()
	TArray<TObjectPtr<ULyraCameraMode>> CameraModeStack;
};


// 카메라의 궁극적인 목표는 모듈화

// FPS, 차, 기본 등 - 이런 것 하나하나를 카메라 모드라고 함.
// ULyraCameraModeStack의 역할 : 
// FPS에서 차로 바로 넘어가면 부자연스럽자너 그래서 TArray 같은 것으로 보관을 해서 스르륵 바뀌도록 함.