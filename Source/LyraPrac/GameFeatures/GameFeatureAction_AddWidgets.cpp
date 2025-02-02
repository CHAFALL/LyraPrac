// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatureAction_AddWidgets.h"
#include "CommonUIExtensions.h"
#include "Components/GameFrameworkComponentManager.h"
#include "LyraPrac/UI/LyraHUD.h"

// FPerContextData -> 어떤 레이아웃, 어떤 위젯을 쓸건지 알 수 있음.
// 이를 통해
void UGameFeatureAction_AddWidgets::AddWidgets(AActor* Actor, FPerContextData& ActiveData)
{
	ALyraHUD* HUD = CastChecked<ALyraHUD>(Actor);

	// HUD를 통해, LocalPlayer을 가져오자
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(HUD->GetOwningPlayerController()->Player))
	{
		// Layout의 요청을 순회하자 (보통 하나만 들어가 있긴하다)
		for (const FLyraHUDLayoutRequest& Entry : Layout)
		{
			// Entry(FLyraHUDLayoutRequest)의 LayoutClass를 보면 TSoftClassPtr(경로 방식)로 되어 있음을 알 수 있다.
			// -> 그럼 로딩해서 넣어줘야 겠지? (inputConfig 참고)
			// 근데 얘는 로딩을 해준 적이 없어서 문제가 발생했음. <- 그럼 해주면 되겠네??
			// -> No!! 액션이라는 것은 클라와 서버 모두 똑같이 발동이 되는데 서버가 UI 관련된 것을 메모리에 올릴 필요가 없음.....
			// 그래서 Lyra에서는 그러한 문제를 해결하기 위해서 클라이언트들만 해당 에셋들을 로딩하도록 
			// 관리를 해줌. 그래서 굳이 Get을 해서 여기에 가져온 것임.
			// 그럼 해결책은?? 
			// -> 이 AddWidget이 들어오기 전에 로딩이 이미 완료가 되어야 겠지? (클라만)
			// -> 어디서? (ExperienceManagerComponent에서 해줄 것임)
			if (TSubclassOf<UCommonActivatableWidget> ConcreteWidgetClass = Entry.LayoutClass.Get()) 
			{
				ActiveData.LayoutsAdded.Add(UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer, Entry.LayerID, ConcreteWidgetClass));
			}
		}

		// 등록이 되는 것은 아니며 관리할 수 있도록 등록만 진행됨.
		// Widget을 순회하며, UIExtensionSubsystem의 Extension에 추가한다
		UUIExtensionSubsystem* ExtensionSubsystem = HUD->GetWorld()->GetSubsystem<UUIExtensionSubsystem>();
		for (const FLyraHUDElementEntry& Entry : Widgets)
		{
			ActiveData.ExtensionHandles.Add(ExtensionSubsystem->RegisterExtensionAsWidgetForContext(Entry.SlotID, LocalPlayer, Entry.WidgetClass.Get(), -1));
		}
	}
}

void UGameFeatureAction_AddWidgets::RemoveWidgets(AActor* Actor, FPerContextData& ActiveData)
{
	ALyraHUD* HUD = CastChecked<ALyraHUD>(Actor);

	// LyraHUD에 추가된 CommonActivatableWidget을 순회하며, Deativate 시켜준다
	for (TWeakObjectPtr<UCommonActivatableWidget>& AddedLayout : ActiveData.LayoutsAdded)
	{
		if (AddedLayout.IsValid())
		{
			AddedLayout->DeactivateWidget();
		}
	}
	ActiveData.LayoutsAdded.Reset();

	// UIExtension에 대해 순회하며, Unregister() 한다
	for (FUIExtensionHandle& Handle : ActiveData.ExtensionHandles)
	{
		// Unregister()는 UUIExtensionSystem에서 제거가 올바르게 되야 한다
		Handle.Unregister();
	}
	ActiveData.ExtensionHandles.Reset();
}

void UGameFeatureAction_AddWidgets::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);
}

void UGameFeatureAction_AddWidgets::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	// GameFrameworkComponentManager를 가져올 GameInstance와 World는 GameWorld여야 한다
	if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
	{
		// GameFrameworkComponentManager를 가져옴
		if (UGameFrameworkComponentManager* ComponentManager = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			// ALyraHUD
			// 전에 모듈러 게임플레이 액터를 만든 이유와 동일하게
			// 리시버를 등록해서 GameFeature의 관련 기술 이용 가능하게 하려고
			// 기본적으로 Widget을 추가할 대상으로 ALyraHUD로 고정한다
			TSoftClassPtr<AActor> HUDActorClass = ALyraHUD::StaticClass();

			// GFA_WorldBase와 마찬가지로 HandleActorExtension을 콜백으로 받도록 하자
			TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle = ComponentManager->AddExtensionHandler(
				HUDActorClass,
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandleActorExtension, ChangeContext));

			ActiveData.ComponentRequests.Add(ExtensionRequestHandle);
		}
	}
}

// 이벤트 네임이랑 액터를 판별
// 액터의 이벤트가 왔으니깐 그 액터에 대해서 따로 랩핑하는 느낌.
// (FPerContextData)
void UGameFeatureAction_AddWidgets::HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext)
{
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	// Receiver인 ALyraHUD의 Removed/Added에 따라 Widget을 추가/제거 해준다
	if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved) || (EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved))
	{
		RemoveWidgets(Actor, ActiveData);
	}
	else if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded) || (EventName == UGameFrameworkComponentManager::NAME_GameActorReady))
	{
		AddWidgets(Actor, ActiveData);
	}
}


