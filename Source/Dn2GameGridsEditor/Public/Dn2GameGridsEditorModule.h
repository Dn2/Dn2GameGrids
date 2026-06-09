#pragma once
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"


class FDn2GameGridsEditorModule : public IModuleInterface
{

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual void PostLoadCallback() override;
	
	void CreateAndSaveMaterialPackage();
#if ENGINE_MAJOR_VERSION == 4
	void OnGridPackageSaved(const FString& PackageFileName, UObject* Outer);
#endif
#if ENGINE_MAJOR_VERSION == 5
	void OnGridPackageSaved(const FString& PackageFileName, UPackage* Package, FObjectPostSaveContext PackageName);
#endif
	
};