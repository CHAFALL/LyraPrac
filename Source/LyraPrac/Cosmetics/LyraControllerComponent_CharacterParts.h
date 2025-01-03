// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraCharacterPartTypes.h"
#include "Components/ControllerComponent.h"
#include "LyraControllerComponent_CharacterParts.generated.h"

class ULyraPawnComponent_CharacterParts;

/** ControllerComponent가 소유하는 Character Parts */
USTRUCT()
struct FLyraControllerCharacterPartEntry
{
	GENERATED_BODY()

	/** Character Part에 대한 정의(메타 데이터 == MetaData) */
	UPROPERTY(EditAnywhere)
	FLyraCharacterPart Part;

	/** Character Part 핸들 (고유값) - Controller가 Possess하고 있는 Pawn에서 생성한(인스턴스) Character Part 핸들값 */
	// 장착여부 관리
	FLyraCharacterPartHandle Handle;
};

UCLASS(meta = (BlueprintSpawnableComponent))
class ULyraControllerComponent_CharacterParts : public UControllerComponent
{
	GENERATED_BODY()
public:
	ULyraControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	ULyraPawnComponent_CharacterParts* GetPawnCustomizer() const;

	UFUNCTION(BlueprintCallable, Category = Cosmetics)
	void AddCharacterPart(const FLyraCharacterPart& NewPart);

	void AddCharacterPartInternal(const FLyraCharacterPart& NewPart);

	void RemoveAllCharacterParts();

	// Good
	// 컨트롤러가 스폰이 되는 시점이 컨트롤러가 Possess가 되는 시점이 아니다.
	// 언제 Possess가 될지는 정확하게 알 수 x
	// 그래서 Possess가 완료가 되었을 때 캐릭터 파츠를 장착시키라고 따로 명령을 또 해줘야 됨..
	// (이를 해결하기 위해 BeginPlay쪽도 이용됨.)
	/** UFUNCTION으로 정의하지 않으면 AddDynamic이 되지 않는다! */
	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	UPROPERTY(EditAnywhere, Category = Cosmetics)
	TArray<FLyraControllerCharacterPartEntry> CharacterParts;
};