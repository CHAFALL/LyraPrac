// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FModularGameplayActorsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};


// ShooterCore에서 Action을 정의하는 곳에 addComponent라는 것이 있음
// 근데 addComponent가 가능한 대상은 AddReceiver라고 GameFrameworkComponentManager에 등록이 되어야 한다.
// 그거를 진행하기 위해서 원래라면 캐릭터, 컨트롤러를 따로 하나 하나 어떤 컴포넌트를 부착시키고 싶은 대상에 대해서 AddReceiver를 다 해줘야 됨...
// -> 그게 귀찮으니깐 ModularGameplayActors라는 자칫 플러그인 방식을 이용할 것임.

// GameFeature의 꽃은 addComponent라고 봐도 됨. (50% ~ 70& 정도 비중.)

// 이렇게 하면 좋은 점이 GameFeature를 쓸거라고 하면 이 ModularCharacter나 ModularPlayerController를 상속 받기만 하면 알아서 등록이 됨!!