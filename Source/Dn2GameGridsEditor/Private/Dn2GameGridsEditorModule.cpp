#include "Dn2GameGridsEditorModule.h"
#include "GameplayTagsManager.h"

IMPLEMENT_GAME_MODULE(FDn2GameGridsEditorModule, Dn2GameGridsEditor);

void FDn2GameGridsEditorModule::StartupModule()
{
	/*
	*	Add GameplayTags that the plugin will always assume is present for making empty grids.
	*	Only remove or edit these if you're changing how ABoxGridActor works.
	*/
	UGameplayTagsManager::Get().AddNativeGameplayTag("Cell.Nav.Void");
	UGameplayTagsManager::Get().AddNativeGameplayTag("Cell.Nav.Blocked");
}


void FDn2GameGridsEditorModule::ShutdownModule()
{

}