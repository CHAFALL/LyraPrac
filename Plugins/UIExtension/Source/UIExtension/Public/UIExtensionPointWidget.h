// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UIExtensionSystem.h"
#include "Components/DynamicEntryBoxBase.h"
#include "UIExtensionPointWidget.generated.h"

/** forward declarations */
class UCommonLocalPlayer;

/**
 * 
 */
UCLASS()
class UIEXTENSION_API UUIExtensionPointWidget : public UDynamicEntryBoxBase
{
	GENERATED_BODY()
public:
	UUIExtensionPointWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void ResetExtensionPoint();
	// Extension Point를 서브시스템에 등록하는 함수
	void RegisterExtensionPoint();
	// PlayerState에 특화된 Extension Point 등록
	void RegisterExtensionPointForPlayerState(UCommonLocalPlayer* LocalPlayer, APlayerState* PlayerState);
	// Extension이 추가/제거될 때 호출되는 콜백
	void OnAddOrRemoveExtension(EUIExtensionAction Action, const FUIExtensionRequest& Request);

	/**
	* UWidget's interfaces
	*/
	// 위젯이 생성되는 시점에 호출됨.
	virtual TSharedRef<SWidget> RebuildWidget() override;

	// 나는 이 태그야
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Extension")
	FGameplayTag ExtensionPointTag;

	// 태그 허용 방식
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Extension")
	EUIExtensionPointMatch ExtensionPointTagMatch = EUIExtensionPointMatch::ExactMatch;

	// 화이트리스트 시스템 느낌
	// 태그만으로 위젯을 찾아서 연결하면, 실제로 연결하려는 위젯의 타입과 호화되지 않아 크래시 발생할 수 있음
	// 이를 방지하기 위해 DataClasses  배열을 통해 허용되는 위젯 클래스들을 미리 지정
	// TObjectPtr<UClass>를 사용해서 유효한 UWidget 파생 클래스만 지정할 수 있도록 타입 안정성을 확보해서
	// 런타임에 호환되지 않는 위젯이 연결되는 것을 방지!
	/** UIExtensionPointWidget에 결합이 허용된 UWidget Classes */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Extension")
	TArray<TObjectPtr<UClass>> DataClasses;

	/** UIExtension --- Widget 관계 매핑 */
	UPROPERTY(Transient)
	TMap<FUIExtensionHandle, TObjectPtr<UUserWidget>> ExtensionMapping;

	/** UIExtensionPoint 객체 관리 */
	TArray<FUIExtensionPointHandle> ExtensionPointHandles;
};
