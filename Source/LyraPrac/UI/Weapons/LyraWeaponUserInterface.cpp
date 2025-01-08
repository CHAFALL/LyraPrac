// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraWeaponUserInterface.h"
#include "LyraPrac/Equipment/LyraEquipmentManagerComponent.h"
#include "LyraPrac/Weapons/LyraWeaponInstance.h"

ULyraWeaponUserInterface::ULyraWeaponUserInterface(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}


// Lyra의 원래 코드에서는 얘가 블루프린트로 빠져있음
// 블루프린트의 경우 Tick이 더 느림. (그럼 조금 덜 계산하겠지?)
// 이 방식 말고 delegate 방식이나 이벤트 방식으로 빼는 것이 나음
void ULyraWeaponUserInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Pawn을 가져오고
	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		// EquipmentManaterComponent를 활용하여, WeaponInstance를 가져오자
		if (ULyraEquipmentManagerComponent* EquipmentManager = Pawn->FindComponentByClass<ULyraEquipmentManagerComponent>())
		{
			if (ULyraWeaponInstance* NewInstance = EquipmentManager->GetFirstInstanceOfType<ULyraWeaponInstance>())
			{
				if (NewInstance != CurrentInstance && NewInstance->GetInstigator() != nullptr)
				{
					// 새로 업데이트해주고, OnWeaponChanged 호출 진행
					ULyraWeaponInstance* OldWeapon = CurrentInstance;
					CurrentInstance = NewInstance;
					OnWeaponChanged(OldWeapon, CurrentInstance);
				}
			}
		}
	}
}
