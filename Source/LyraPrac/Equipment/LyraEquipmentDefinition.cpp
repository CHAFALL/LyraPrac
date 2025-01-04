// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraEquipmentDefinition.h"
#include "LyraEquipmentInstance.h"

ULyraEquipmentDefinition::ULyraEquipmentDefinition(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// 기본값으로, LyraEquipmentInstance로 설정
	InstanceType = ULyraEquipmentInstance::StaticClass();
}
