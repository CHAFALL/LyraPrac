// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "LyraActivatableWidget.generated.h"

/**
 * Input 처리 방식 정의
 */
UENUM(BlueprintType)
enum class ELyraWidgetInputMode : uint8
{
	Default,
	GameAndMenu, // 메뉴와 게임 상황 동시인 경우
	Game,
	Menu,
};

/**
 * CommonActivatableWidget의 주석을 보면, 복잡하게 설명되어 있다.
 * 필자가 이해한 CommonActivatableWidget은 두가지 특성을 가지고 있다:
 * 1. Widget Layout과 Widget Instance을 구분하기 위한 용도로 CommonActivatableWidget은 Layout 정의:
 *    - 런타임 Activate/Deactivate
 *    - WidgetTree에서 제거가 아닌 껐다/켰다(== Activate/Deactivate)
 * 2. Input(Mouse or Keyboard etc...) 처리 방법 정의
 */
UCLASS()
class LYRAPRAC_API ULyraActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	ULyraActivatableWidget(const FObjectInitializer& ObjectInitializer);

	/**
	* UCommonActivatableWidget interfaces
	*/
	// 위젯이 활성화 되는 순간에 바로 발동.
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

	/** Input 처리 방식*/
	UPROPERTY(EditDefaultsOnly, Category = Input)
	ELyraWidgetInputMode InputConfig = ELyraWidgetInputMode::Default;

	/** Mouse 처리 방식*/
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
};
