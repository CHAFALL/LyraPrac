// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraInventoryManagerComponent.h"
#include "LyraInventoryItemDefinition.h"
#include "LyraInventoryItemInstance.h"

ULyraInventoryItemInstance* FLyraInventoryList::AddEntry(TSubclassOf<ULyraInventoryItemDefinition> ItemDef)
{
	ULyraInventoryItemInstance* Result = nullptr;
	check(ItemDef);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner(); // ULyraInventoryManagerComponent -> UActorComponent인데 이게 컨트롤러에 부착되었으므로 결론적으론 컨트롤러가 Owner
	check(OwningActor->HasAuthority());

	FLyraInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<ULyraInventoryItemInstance>(OwningActor);
	NewEntry.Instance->ItemDef = ItemDef;

	// (추가)
	// iterating fragments and call callback to OnInstanceCreated()
	for (const ULyraInventoryItemFragment* Fragment : GetDefault<ULyraInventoryItemDefinition>(ItemDef)->Fragments)
	{
		if (Fragment)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}


	Result = NewEntry.Instance;
	return Result;
}

ULyraInventoryManagerComponent::ULyraInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{}

ULyraInventoryItemInstance* ULyraInventoryManagerComponent::AddItemDefinition(TSubclassOf<ULyraInventoryItemDefinition> ItemDef)
{
	ULyraInventoryItemInstance* Result = nullptr;
	if (ItemDef)
	{
		Result = InventoryList.AddEntry(ItemDef);
	}
	return Result;
}


