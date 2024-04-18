// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayTagsManager.h"
#include "Dn2GameGrids.h"

#define LOCTEXT_NAMESPACE "FDn2GameGridsModule"

void FDn2GameGridsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	/*
	*	Add GameplayTags that the plugin will always assume is present for making empty grids.
	*	Only remove or edit these if you're changing how ABoxGridActor works.
	*/
	UGameplayTagsManager::Get().AddNativeGameplayTag("Cell.Nav.Void");
	UGameplayTagsManager::Get().AddNativeGameplayTag("Cell.Nav.Blocked");
}

void FDn2GameGridsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDn2GameGridsModule, Dn2GameGrids)