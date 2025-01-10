// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraGameplayTagStack.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraGameplayTagStack)

void FLyraGameplayTagStackContainer::AddStack(FGameplayTag Tag, int32 StackCount)
{
	if (!Tag.IsValid())
	{
		return;
	}

	if (StackCount > 0)
	{
		// linear search...
		// - we can't say this is performant, but my guess is that the number of Stacks should be less than dozens
		for (FLyraGameplayTagStack& Stack : Stacks)
		{
			if (Stack.Tag == Tag)
			{
				const int32 NewCount = Stack.StackCount + StackCount;
				Stack.StackCount = NewCount;
				TagToCountMap[Tag] = NewCount;
				return;
			}
		}

		FLyraGameplayTagStack& NewStack = Stacks.Emplace_GetRef(Tag, StackCount);

		// if we reach to this line of code, the initial StackCount is 0
		TagToCountMap.Add(Tag, StackCount);
	}
}

void FLyraGameplayTagStackContainer::RemoveStack(FGameplayTag Tag, int32 StackCount)
{
	if (!Tag.IsValid())
	{
		return;
	}

	if (StackCount > 0)
	{
		// we use Iterator pattern to search, cuz it is more convenient to erase elements while iterating
		for (auto It = Stacks.CreateIterator(); It; ++It)
		{
			FLyraGameplayTagStack& Stack = *It;
			if (Stack.Tag == Tag)
			{
				// we reach to zero (apparently less than zero)
				if (Stack.StackCount <= StackCount)
				{
					// THIS IS THE WAY TO DELETE ELEMENT WHILE WE ITERATE
					It.RemoveCurrent();
					TagToCountMap.Remove(Tag);
				}
				// just update normally
				else
				{
					const int32 NewCount = Stack.StackCount - StackCount;
					Stack.StackCount = NewCount;
					TagToCountMap[Tag] = NewCount;
				}
				return;
			}
		}
	}
}
