// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularCharacter.h"
#include "Components/GameFrameworkComponentManager.h"

void AModularCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	// UGameFrameworkComponentManager에다가 Receiver 등록을 해서 GameFeature에서 addComponent라던가 그런 기능을 얘도 수신할 수 있도록 등록
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AModularCharacter::BeginPlay()
{
	// 참고. HeroComponent에서 타이밍 이슈때문에 해줬던거랑 같이 생각? 하면 좋을듯?
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::BeginPlay();
}

void AModularCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 이거 순서 잘 해줘야 됨!!!
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}


