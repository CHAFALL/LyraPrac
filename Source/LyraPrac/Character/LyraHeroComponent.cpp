// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraHeroComponent.h"
#include "LyraPawnData.h"
#include "LyraPawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "LyraPrac/LyraGameplayTags.h"
#include "LyraPrac/LyraLogChannels.h"
#include "LyraPrac/Player/LyraPlayerState.h"
#include "LyraPrac/Camera/LyraCameraComponent.h"

/** FeatureName 정의: static member variable 초기화 */
const FName ULyraHeroComponent::NAME_ActorFeatureName("Hero");

ULyraHeroComponent::ULyraHeroComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void ULyraHeroComponent::OnRegister()
{
	Super::OnRegister();

	// 올바른 Actor에 등록되었는지 확인:
	{
		if (!GetPawn<APawn>())
		{
			UE_LOG(LogLyra, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
			return;
		}
	}

	RegisterInitStateFeature();
}

void ULyraHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// (차이점)
	// 얘는 ULyraPawnExtensionComponent의 변화만 알면 된다.
	// 너 이제 다음 단계로 넘어가라는 명령만 확인하면 됨.
	// PawnExtensionComponent에 대해서 (PawnExtension Feature) OnActorInitStateChanged() 관찰하도록 (Observing)
	BindOnActorInitStateChanged(ULyraPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// InitState_Spawned로 초기화
	ensure(TryToChangeInitState(FLyraGameplayTags::Get().InitState_Spawned));

	// ForceUpdate 진행
	CheckDefaultInitialization();
}

void ULyraHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void ULyraHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	const FLyraGameplayTags& InitTags = FLyraGameplayTags::Get();

	if (Params.FeatureName == ULyraPawnExtensionComponent::NAME_ActorFeatureName)
	{
		// LyraPawnExtensionComponent의 DataInitialized 상태 변화 관찰 후, LyraHeroComponent도 DataInitialized 상태로 변경
		// - CanChangeInitState 확인
		if (Params.FeatureState == InitTags.InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

// (고민) 넘어갈 수 있는 때가 되면 누가 알려줘? - ULyraPawnExtensionComponent의 OnActorInitStateChanged를 참고할 것
// 그리고 Hero의 OnActorInitStateChanged도 보면서 어떤 것이 다른지 파악할 것.
bool ULyraHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	const FLyraGameplayTags& InitTags = FLyraGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();
	ALyraPlayerState* LyraPS = GetPlayerState<ALyraPlayerState>();

	// Spawned 초기화
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		if (Pawn)
		{
			return true;
		}
	}

	// Spawned -> DataAvailable
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		// 여기서도 PlayerState가 없으면 업데이트가 진행이 되지 않음 (Possess가 되고 난 뒤가 되어야 됨....)
		// (왜냐하면 PlayerState란 것 자체가 플레이어 컨트롤러에 대한 정보들을 저장하기 위한 공간이므로 Possess가 되어야지 PlayerState가 업데이트가 됨)
		// 그래서 누군가는 Possess가 되고 난 뒤에 Extension의 업데이트를 호출해줘야 됨.
		if (!LyraPS)
		{
			return false;
		}

		return true;
	}

	// DataAvailable -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// PawnExtensionComponent가 DataInitialized될 때까지 기다림 (== 모든 Feature Component가 DataAvailable인 상태)
		// (차이점)
		return LyraPS && Manager->HasFeatureReachedInitState(Pawn, ULyraPawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
	}

	// DataInitialized -> GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void ULyraHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	const FLyraGameplayTags& InitTags = FLyraGameplayTags::Get();

	// DataAvailable -> DataInitialized 단계
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ALyraPlayerState* LyraPS = GetPlayerState<ALyraPlayerState>();
		if (!ensure(Pawn && LyraPS))
		{
			return;
		}

		// Input과 Camera에 대한 핸들링... (TODO)

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const ULyraPawnData* PawnData = nullptr;
		if (ULyraPawnExtensionComponent* PawnExtComp = ULyraPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<ULyraPawnData>();
		}

		if (bIsLocallyControlled && PawnData)
		{
			// 현재 LyraCharacter에 Attach된 CameraComponent를 찾음
			if (ULyraCameraComponent* CameraComponent = ULyraCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}
	}
}

void ULyraHeroComponent::CheckDefaultInitialization()
{
	// (차이점)
	// 앞서 BindOnActorInitStateChanged에서 보았듯이 Hero Feature는 Pawn Extension Feature에 종속되어 있으므로, CheckDefaultInitializationForImplementers 호출하지 않음:
	
	// 복습:
	/*
	* CheckDefaultInitializationForImplementers는 자신을 제외한 모든 연결된 컴포넌트들에게
	* CheckDefaultInitialization()을 호출하는 역할을 수행합니다.
	*
	* Extension 컴포넌트에서 CheckDefaultInitializationForImplementers를 호출하면:
	* 1. Init state 체인에 연결된 모든 컴포넌트들이 CheckDefaultInitialization()을 호출
	* 2. 각 컴포넌트는 자신의 상태를 파악할 때까지 내부적으로 업데이트를 수행 (강제 업데이트)
	*
	* 이 방식의 장점:
	* - PawnExtension 컴포넌트가 다른 컴포넌트의 존재 여부를 알 필요가 없음 (낮은 종속성)
	* - "내 밑에 있는 컴포넌트들은 알아서 업데이트하라"는 방식으로 동작
	*/

	// ContinueInitStateChain은 앞서 PawnExtComponent와 같음
	const FLyraGameplayTags& InitTags = FLyraGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };
	ContinueInitStateChain(StateChain);
}

PRAGMA_DISABLE_OPTIMIZATION
TSubclassOf<ULyraCameraMode> ULyraHeroComponent::DetermineCameraMode() const
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	if (ULyraPawnExtensionComponent* PawnExtComp = ULyraPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const ULyraPawnData* PawnData = PawnExtComp->GetPawnData<ULyraPawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}
PRAGMA_ENABLE_OPTIMIZATION