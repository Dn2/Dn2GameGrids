// Copyright Epic Games, Inc. All Rights Reserved.

#include "Dn2GameGrids.h"

#include "AssetToolsModule.h"
#include "GridMapData.h"
#include "IAssetTools.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "Engine/AssetManager.h"
#include "Subsystems/AssetEditorSubsystem.h"


#define LOCTEXT_NAMESPACE "FDn2GameGridsModule"


void FDn2GameGridsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

#if WITH_EDITOR
	if (GIsEditor && !FApp::IsGame())
	{
		FModuleManager::Get().GetModule("Dn2GameGridsEditor")->PostLoadCallback();
	}

#endif
}

void FDn2GameGridsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDn2GameGridsModule, Dn2GameGrids)