#pragma once
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"


class FDn2GameGridsEditorModule : public IModuleInterface
{

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};