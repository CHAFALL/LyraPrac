// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraPlayerState.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "LyraPrac/AbilitySystem/LyraAbilitySet.h"
#include "LyraPrac/AbilitySystem/LyraAbilitySystemComponent.h"
#include "LyraPrac/AbilitySystem/Attributes/LyraCombatSet.h"
#include "LyraPrac/AbilitySystem/Attributes/LyraHealthSet.h"
#include "LyraPrac/Character/LyraPawnData.h"
#include "LyraPrac/GameModes/LyraExperienceManagerComponent.h"
#include "LyraPrac/GameModes/LyraGameModeBase.h"

ALyraPlayerState::ALyraPlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<ULyraAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	// 어? 캐싱을 안하네??
	// -> AbilitySystemComponent이 Init 되는 과정에서 Outer(PlayerState)에 있는 Attribute를 알아서 다 가져옴!
	// 그다음에 AbilitySystemComponent가 알아서 저장함.
	// 그래서 반드시 Attribute는 AbilitySystem이 생성되는 위치에 같이 있어야 한다! (Outer가 동일해야 됨!!)
	CreateDefaultSubobject<ULyraHealthSet>(TEXT("HealthSet"));
	CreateDefaultSubobject<ULyraCombatSet>(TEXT("CombatSet")); 

}

void ALyraPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 추가 (어빌리티)
	check(AbilitySystemComponent);
	// 아래의 코드는 우리가 InitAbilityActorInfo를 재호출을 통하는 이유를 설명하는 코드이다:
	{
		// 처음 InitAbilityActorInfo를 호출 당시, OwnerActor와 AvatarActo가 같은 Actor를 가르키고 있으며, 이는 PlayerState이다
		// - OwnerActor는 PlayerState가 의도하는게 맞지만, AvatarActor는 PlayerController가 소유하는 대상인 Pawn이 되어야 한다!
		// - 이를 위해 재-세팅을 해준다
		FGameplayAbilityActorInfo* ActorInfo = AbilitySystemComponent->AbilityActorInfo.Get();
		check(ActorInfo->OwnerActor == this);
		check(ActorInfo->OwnerActor == ActorInfo->AvatarActor);
	}
	// 이게 중요한 이유: 어빌리티 시스템 같은 경우 생성이 되고 끝나는 것이 아니라
	// 누가 소유자고 아바타가 뭔지를 결정해줘야 됨.
	// 근데 해당 시점은 극 초반이라서 Pawn이 Possess가 안되어있어서 Owner는 PlayerState로 잘 들어가지만 아바타는 nullptr로 들어감... (나중에 재 세팅 필요!)
	// -> 그럼 어디서?? HeroComponent에서 해줄 것임.
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

	const AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);

	ULyraExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnLyraExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void ALyraPlayerState::OnExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience)
{
	if (ALyraGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ALyraGameModeBase>())
	{
		// ALyraGameMode에서 GetPawnDataForController를 구현해야 함
		// - GetPawnDataForController에서 우리는 아직 PawnData를 설정하지 않았으므로, ExperienceMangerComponent의 DefaultPawnData로 설정한다
		const ULyraPawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);

		SetPawnData(NewPawnData);
	}
}

void ALyraPlayerState::SetPawnData(const ULyraPawnData* InPawnData)
{
	check(InPawnData);

	// PawnData가 두번 설정되는 것은 원하지 않음!
	check(!PawnData);

	PawnData = InPawnData;

	// PawnData의 AbilitySet을 순회하며, ASC에 Ability를 할당(Give)한다
	// - 이 과정에서 ASC의 ActivatableAbilities에 추가된다
	for (ULyraAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}
}
