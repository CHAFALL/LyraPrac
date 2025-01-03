// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LyraCharacterPartTypes.generated.h"


USTRUCT(BlueprintType)
struct FLyraCharacterPartHandle
{
	GENERATED_BODY()

	void Reset()
	{
		PartHandle = INDEX_NONE;
	}

	bool IsValid() const
	{
		return PartHandle != INDEX_NONE;
	}

	UPROPERTY()
	int32 PartHandle = INDEX_NONE;
};


USTRUCT(BlueprintType)
struct FLyraCharacterPart
{
	GENERATED_BODY()

	/** 파츠로 생성된 Actor의 Class 정보를 가지고 있음 -> Actor BP를 하나의 파츠로 생각하면 됨 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PartClass;

	/** Actor를 소환하여 붙이는데 특정 Bone에 붙이기 위한 정보 (따로 정의되어 있지 않으면 그냥 Root에 붙임) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
};




// 어떤 액터를 장착할 때 웬만하면 캐릭터이다보니 루트에다가 하지만,
// 혹시나 소켓안에다가 장착하고 싶을 수 있으므로

// 중요한 점
// TObjectPtr이 아닌 TSubclassOf로 되어있다는 것은 메타데이터로 들고 있다는 것이고 
// 메타데이터로 들고 있다는 것은 굉장히 가볍다는 뜻.

// 참고로 TSubclassOf의 단점은?
// 실제 인스턴스에 접근 불가
// 객체의 상태를 직접 다룰 수 없음
// 실제 사용하려면 결국 인스턴스를 생성해야 함