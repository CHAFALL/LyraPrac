// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "Templates/SharedPointer.h"
#include "Subsystems/WorldSubsystem.h"
#include "UIExtensionSystem.generated.h"

class UUIExtensionSubsystem;

/** ExtensionPoint GameplayTag 비교 방법 */
UENUM(BlueprintType)
enum class EUIExtensionPointMatch : uint8
{
	/** A.B는 A.B.C를 ***허용하지*** 않는다 */
	ExactMatch,

	/** A.B는 A.B.C를 ***허용*** 한다 */
	PartialMatch,
};

UENUM(BLueprintType)
enum class EUIExtensionAction : uint8
{
	Added,
	Removed,
};

// FUIExtension : 레이아웃 안에 있는 또 하나의 위젯
// 따라서 당연히 태그가 필요.
// 태그랑 위젯이랑 맵핑 시킨 느낌.
struct FUIExtension : TSharedFromThis<FUIExtension>
{
	/** UIExtension Widget의 Point Tag이다 (무슨 의미인지 하나씩 구현내가며 이해해보자) */
	FGameplayTag ExtensionPointTag;

	/** Widget Class를 가지고 있으며, UUIExtensionSubsystem에서 AddReferencedObjects에서 GC를 막는다 */
	UObject* Data = nullptr;

	/** 보통 LocalPlayer로 설정된다 */
	TWeakObjectPtr<UObject> ContextObject;

	int32 Priority = INDEX_NONE;
};

USTRUCT(BlueprintType)
struct UIEXTENSION_API FUIExtensionHandle
{
	GENERATED_BODY()
public:
	FUIExtensionHandle() {}
	FUIExtensionHandle(UUIExtensionSubsystem* InExtensionSource, const TSharedPtr<FUIExtension>& InDataPtr)
		: ExtensionSource(InExtensionSource)
		, DataPtr(InDataPtr)
	{}

	void Unregister();
	bool IsValid() const { return DataPtr.IsValid(); }
	bool operator==(const FUIExtensionHandle& Other) const { return DataPtr == Other.DataPtr; }
	bool operator!=(const FUIExtensionHandle& Other) const { return !operator==(Other); }

	// 주소값 같은 경우 당연히 고유성이 보장이 됨.
	// 해시는 가장 중요한 것이 고유성 보장
	// 그래서 일반저긍로 포인터를 이용해서 작공하는 것들은 이렇게 포인터 해시를 많이 이용.
	friend FORCEINLINE uint32 GetTypeHash(FUIExtensionHandle Handle)
	{
		return PointerHash(Handle.DataPtr.Get());
	}

	friend class UUIExtensionSubsystem;
	TWeakObjectPtr<UUIExtensionSubsystem> ExtensionSource; // 월드마다 다를 것이므로
	TSharedPtr<FUIExtension> DataPtr;
};

// 태그도 맞고 허용된 클래스이면 요청을 보냄.
USTRUCT(BlueprintType)
struct FUIExtensionRequest
{
	GENERATED_BODY()
public:
	/** UIExtensionPoint와 연동될 Extension */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FUIExtensionHandle ExtensionHandle;

	/** Extension의 Slot GameplayTag */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ExtensionPointTag;

	/** WidgetClass로 FUIExtension과 같다 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UObject> Data = nullptr;

	/** FUIExtension의 ContextObject를 전달받음 (UCommonLocalPlayer, UModularPlayerState, ... == UIExtension을 실행한 Instigator) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UObject> ContextObject = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Priority = INDEX_NONE;
};

DECLARE_DELEGATE_TwoParams(FExtendExtensionPointDelegate, EUIExtensionAction Action, const FUIExtensionRequest& Request);


struct FUIExtensionPoint : TSharedFromThis<FUIExtensionPoint>
{
public:
	/** Extension---ExtensionPoint와 매칭되는지 확인 */
	bool DoesExtensionPassContract(const FUIExtension* Extension) const;

	// 나는 이런 태그야
	/** UIExtension의 Slot GameplayTag */
	FGameplayTag ExtensionPointTag;

	/** UIExtension을 생성/제거한 Instigator(주체) */
	TWeakObjectPtr<UObject> ContextObject;

	/** UIExtensionPointWidget에 허용된 Widget Class -> UIExtensionPointWidget's DataClasses */
	TArray<UClass*> AllowedDataClasses;

	/** Widget을 ExtensionPointWidget에 연결하기 위한 Callback 함수 */
	FExtendExtensionPointDelegate Callback;
	EUIExtensionPointMatch ExtensionPointTagMatchType = EUIExtensionPointMatch::ExactMatch;
};

// FUIExtensionPoint랑 UUIExtensionSubsystem를 매핑시키기 위함.
USTRUCT(BlueprintType)
struct UIEXTENSION_API FUIExtensionPointHandle
{
	GENERATED_BODY()
public:
	FUIExtensionPointHandle() {}
	FUIExtensionPointHandle(UUIExtensionSubsystem* InExtensionSource, const TSharedPtr<FUIExtensionPoint>& InDataPtr)
		: ExtensionSource(InExtensionSource), DataPtr(InDataPtr)
	{}

	void Unregister();
	bool IsValid() const { return DataPtr.IsValid(); }
	bool operator==(const FUIExtensionPointHandle& Other) const { return DataPtr == Other.DataPtr; }
	bool operator!=(const FUIExtensionPointHandle& Other) const { return !operator==(Other); }

	TWeakObjectPtr<UUIExtensionSubsystem> ExtensionSource;
	TSharedPtr<FUIExtensionPoint> DataPtr;
};

template <>
struct TStructOpsTypeTraits<FUIExtensionHandle> : public TStructOpsTypeTraitsBase2<FUIExtensionHandle>
{
	enum
	{
		WithCopy = true,
		WithIdenticalViaEquality = true,
	};
};

UCLASS()
class UIEXTENSION_API UUIExtensionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	void UnregisterExtension(const FUIExtensionHandle& ExtensionHandle);
	FUIExtensionHandle RegisterExtensionAsWidgetForContext(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, TSubclassOf<UUserWidget> WidgetClass, int32 Priority);
	FUIExtensionHandle RegisterExtensionAsData(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, UObject* Data, int32 Priority);

	void UnregisterExtensionPoint(const FUIExtensionPointHandle& ExtensionPointHandle);
	FUIExtensionPointHandle RegisterExtensionPointForContext(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, EUIExtensionPointMatch ExtensionPointTagMatchType, const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback);
	FUIExtensionPointHandle RegisterExtensionPoint(const FGameplayTag& ExtensionPointTag, EUIExtensionPointMatch ExtensionPointTagMatchType, const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback);

	FUIExtensionRequest CreateExtensionRequest(const TSharedPtr<FUIExtension>& Extension);


	/** ExtensionPoint --(Broadcast)--> Extensions [ExtensionPointrk 추가/제거 되었을 경우, Extension에 알림: 참고로 Added만 함] */
	void NotifyExtensionPointOfExtensions(TSharedPtr<FUIExtensionPoint>& ExtensionPoint);
	/** Extension --(Broadcast) --> ExtensionPoints [Extension이 추가/제거 되었을 경우, Extension Points에 알림] */
	void NotifyExtensionPointsOfExtension(EUIExtensionAction Action, TSharedPtr<FUIExtension>& Extension);

	/** GameplayTag(Slot) --- FUIExtension(WidgetClass) */
	typedef TArray<TSharedPtr<FUIExtension>> FExtensionList;
	TMap<FGameplayTag, FExtensionList> ExtensionMap;

	/** GameplayTag(Slot) --- FUIExtensionPoint(WidgetClassWithContext) */
	typedef TArray<TSharedPtr<FUIExtensionPoint>> FExtensionPointList;
	TMap<FGameplayTag, FExtensionPointList> ExtensionPointMap;

};


// 포인트 위젯이 생성되고 난 뒤에 서브 시스템에 요청을 보냄
// 나 이거 만들거야! (그게 바로  FUIExtensionPoint임)
// 그럼 UI에서 Handle로 관리를 하고 핸들을 통해서 내부적으로 요청이 올때마다 나 이거 장착시켜줘
// 일반 UIExtension들이 addWidget을 통해서 장착 요청을 보낼 것이다
// 그때 포인트랑 비교해서 맞으면 Request를 생성해서 보내는 방식이다.

// GPT
/*
*	UIExtensionPointWidget 생성 단계
	-UIExtensionPointWidget이 생성되면 서브시스템에 자신의 존재를 등록합니다
	-이때 FUIExtensionPoint를 통해 "나는 이런 태그를 가진 위젯을 받을 수 있어"라고 알립니다
	-이 정보는 핸들(FUIExtensionPointHandle)을 통해 관리됩니다

	확장(Extension) 요청 처리
	-다른 UIExtension들이 addWidget을 통해 위젯 추가를 요청합니다
	-이때 시스템은:
		-태그를 비교하고
		-DataClasses와의 호환성을 확인하고
		-조건이 맞으면 FUIExtensionRequest를 생성해서 처리합니다
	
	관리 방식
	-모든 과정은 핸들을 통해 관리됩니다
	-이를 통해 안전한 참조와 제거가 가능합니다
*/


// Extension이 추가가 된 시점에 ExtensionPoint를 검색하는 버전과
// ExtensionPoint가 추가가 된 시점에 Extension를 검색하는 버전 2가지 다 있음.
// 방식이 2개로 나뉜 이유 - 순서 보장 x 하므로. (Subsystem으로 뺀 이유 중에 하나이기도 함.)

// 정리
// 강의 PointWidget #2의 12분 부터