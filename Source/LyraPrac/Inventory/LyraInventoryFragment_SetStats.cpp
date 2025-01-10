// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraInventoryFragment_SetStats.h"
#include "LyraInventoryItemInstance.h"

void ULyraInventoryFragment_SetStats::OnInstanceCreated(ULyraInventoryItemInstance* Instance) const
{
	// iterating InitialItemStats and add stat tag to InventoryItemInstance
	for (const auto& InitialItemStat : InitialItemStats)
	{
		Instance->AddStatTagStack(InitialItemStat.Key, InitialItemStat.Value);
	}
}
