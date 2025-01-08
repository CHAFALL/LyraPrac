// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonLocalPlayer.h"

// Experience에서 했던 방식과 동일함을 알 수 있음
FDelegateHandle UCommonLocalPlayer::CallAndRegister_OnPlayerStateSet(FPlayerStateSetDelegate::FDelegate Delegate)
{
	APlayerController* PC = GetPlayerController(GetWorld());
	APlayerState* PlayerState = PC ? PC->PlayerState : nullptr;
	if (PlayerState)
	{
		// PlayerState가 설정되어 있으면 바로 호출
		Delegate.Execute(this, PlayerState);
	}
	// OnPlayerStateSet에 등록
	// 그럼 어느 시점에서 이 델리게이트를 호출해줘야 될까??? 
	// -> CommonPlayerController쪽에 있음.
	return OnPlayerStateSet.Add(Delegate);
}
