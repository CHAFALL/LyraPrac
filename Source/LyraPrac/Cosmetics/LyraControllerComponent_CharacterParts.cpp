// Fill out your copyright notice in the Description page of Project Settings.

#include "LyraControllerComponent_CharacterParts.h"
#include "LyraPawnComponent_CharacterParts.h"


ULyraControllerComponent_CharacterParts::ULyraControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void ULyraControllerComponent_CharacterParts::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (AController* OwningController = GetController<AController>())
		{
			// OnPossessedPawnChanged라는 델리게이트 이용.
			// Possess하고 있는 Pawn이 변경될 때마다 우리가 설정한 함수를 불러줌.
			OwningController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);
		}
	}
}

void ULyraControllerComponent_CharacterParts::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveAllCharacterParts();
	Super::EndPlay(EndPlayReason);
}


// ControllerComponent라는 것이 Controller에 부착이 된다는 뜻이고 부착이 됨과 동시에 부착되는
// 대상에 대한 유틸 함수를 많이 이용 가능! (ex. GetPawn)
PRAGMA_DISABLE_OPTIMIZATION
ULyraPawnComponent_CharacterParts* ULyraControllerComponent_CharacterParts::GetPawnCustomizer() const
{
	// 내 컨트롤러가 빙의한 폰을 가져옴
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// 생각해보면, 우리는 앞서 LyraPawnComponent_CharacterParts를 상속받는 B_MannequinPawnCosmetics를 이미 B_Hero_ShooterMannequin에 추가하였다.
		// B_MannequinPawnCosmetics를 반환되길 기대한다.
		// 폰에다가 붙여놨던 ULyraPawnComponent_CharacterParts를 가져옴.
		return ControlledPawn->FindComponentByClass<ULyraPawnComponent_CharacterParts>();
	}
	return nullptr;
}
PRAGMA_ENABLE_OPTIMIZATION

// (cf.)왜 굳이 이렇게 할까? -> 그러면 데디 서버 고려를 의심해볼것!
// Lyra에선 AddCharacterPartInternal(NewPart)말고 딴 부분도 있었음..
void ULyraControllerComponent_CharacterParts::AddCharacterPart(const FLyraCharacterPart& NewPart)
{
	AddCharacterPartInternal(NewPart);
}

void ULyraControllerComponent_CharacterParts::AddCharacterPartInternal(const FLyraCharacterPart& NewPart)
{
	// AddDefaulted_GetRef -> 배열에 새로운 공간을 확보하고그 공간을 기본값으로 초기화하고 그 공간의 참조(reference)를 반환. (최적화)
	// 원래라면 파츠 엔트리를 만든 후에 데이터를 넣고 그 다음에 캐릭터 파츠에다가 넘겨줘야 했음(복사 연산 필요.) - 그 과정을 없앤 것임. 
	FLyraControllerCharacterPartEntry& NewEntry = CharacterParts.AddDefaulted_GetRef();
	NewEntry.Part = NewPart;

	// 아직 Possess가 되지 않았음....
	if (ULyraPawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		// ULyraPawnComponent_CharacterParts에서 AddCharacterPart를 한다는 것은 실제로 스폰한다는 뜻.
		NewEntry.Handle = PawnCustomizer->AddCharacterPart(NewPart);
	}
}

void ULyraControllerComponent_CharacterParts::RemoveAllCharacterParts()
{
	if (ULyraPawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		for (FLyraControllerCharacterPartEntry& Entry : CharacterParts)
		{
			PawnCustomizer->RemoveCharacterPart(Entry.Handle);
		}
	}
	CharacterParts.Reset();
}

void ULyraControllerComponent_CharacterParts::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	// 이전 OldPawn에 대해서는 Character Parts를 전부 제거해주자
	if (ULyraPawnComponent_CharacterParts* OldCustomizer = OldPawn ? OldPawn->FindComponentByClass<ULyraPawnComponent_CharacterParts>() : nullptr)
	{
		for (FLyraControllerCharacterPartEntry& Entry : CharacterParts)
		{
			OldCustomizer->RemoveCharacterPart(Entry.Handle);
			Entry.Handle.Reset();
		}
	}

	// 새로운 Pawn에 대해서 기존 Controller가 가지고 있는 Character Parts를 추가해주자
	if (ULyraPawnComponent_CharacterParts* NewCustomizer = NewPawn ? NewPawn->FindComponentByClass<ULyraPawnComponent_CharacterParts>() : nullptr)
	{
		for (FLyraControllerCharacterPartEntry& Entry : CharacterParts)
		{
			check(!Entry.Handle.IsValid());
			Entry.Handle = NewCustomizer->AddCharacterPart(Entry.Part);
		}
	}
}
