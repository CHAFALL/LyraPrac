// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraPrac.h"
#include "LyraLogChannels.h"
#include "Modules/ModuleManager.h"

// 기본 모듈을 상속 받아서 커스텀 모듈 생성 (이런게 있다 정도로만)
class FLyraPracModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

void FLyraPracModule::StartupModule()
{
	FDefaultGameModuleImpl::StartupModule(); // 디버깅을 해보면 에디터가 시작하기도 전에 걸림.
	UE_LOG(LogLyra, Warning, TEXT("FLyraPracModule"));
}

void FLyraPracModule::ShutdownModule()
{
	FDefaultGameModuleImpl::ShutdownModule();
}

// 어떤 모듈을 사용할 것인지 말해주는 곳
// (라이라) 에디터쪽 보면 IMPLEMENT_PRIMARY_GAME_MODULE가 아닌 IMPLEMENT_MODULE로 되어있음
// IMPLEMENT_PRIMARY_GAME_MODULE는 반드시 한개만 존재해야됨 (에러명을 찾을 수도 없음...) - 핵심이 되는 애한테만 쓸 것!
// 그 이후에는 각 모듈마다 IMPLEMENT_MODULE로 설정을 해줘야 됨.. (이거 없어도 에러뜸)
IMPLEMENT_PRIMARY_GAME_MODULE(FLyraPracModule, LyraPrac, "LyraPrac" );


