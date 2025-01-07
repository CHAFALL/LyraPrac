// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraHUD.h"
#include "Components/GameFrameworkComponentManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraHUD)

ALyraHUD::ALyraHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void ALyraHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	// LyraHUD를 Receiver로 Actor를 추가하자
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

//
void ALyraHUD::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::BeginPlay();
}

void ALyraHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}
