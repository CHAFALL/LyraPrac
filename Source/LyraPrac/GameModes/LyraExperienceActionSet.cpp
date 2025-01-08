// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraExperienceActionSet.h"
#include "GameFeatureAction.h"

ULyraExperienceActionSet::ULyraExperienceActionSet()
{
}

#if WITH_EDITORONLY_DATA
void ULyraExperienceActionSet::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();

	for (UGameFeatureAction* Action : Actions)
	{
		if (Action)
		{
			Action->AddAdditionalAssetBundleData(AssetBundleData);
		}
	}
}
#endif