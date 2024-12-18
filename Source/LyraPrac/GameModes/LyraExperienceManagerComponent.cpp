// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraExperienceManagerComponent.h"

// 내가 물어보는 시점에 이미 experience 로딩이 완료가 되어있으면 불필요한 처리를 줄임.
void ULyraExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnLyraExperienceLoaded::FDelegate&& Delegate)
{
	// IsExperienceLoaded() 구현
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		/**
		 * 참고로, 여러분들은 Delegate 객체를 자세히 살펴보면, 내부적으로 필요한 변수들은 메모리 할당해놓는다:
		 * TArray<int> a = {1, 2, 3, 4};
		 * delegate_type delegate = [a](){
		 *	return a.Num();
		 * }
		 * a는 delegate_type 내부에 new로 할당되어 있다. 복사 비용을 낮추기 위해 Move를 통해 하는 것을 잊지 말자!
		 */
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}
