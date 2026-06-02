// Fill out your copyright notice in the Description page of Project Settings.


#include "GridMapDataFactoryNew.h"
#include "GridMapData.h"


UGridMapDataFactoryNew::UGridMapDataFactoryNew(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Tell the editor which type of assets this factor can
	// create.
	SupportedClass = UGridMapData::StaticClass();

	// This factory creates new instances from scratch
	// rather than importing using drag-and-drop. This is
	// what decide which of the three factory types this
	// factory is. How we can get three types from a bool is
	// beyond me.
	bCreateNew = true;

	// Enter name-edit-mode after the asset has been created.
	bEditAfterNew = true;
}

// Called by the engine when a new instance of the asset
// type is to be created. That is, when the  user has
// right-clicked in the Content Browser and selected MyAsset
// from the list.
UObject* UGridMapDataFactoryNew::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(InClass->IsChildOf(UGridMapData::StaticClass()));
	return NewObject<UGridMapData>(InParent, InClass, InName, Flags);
}

// Return true to make the MyAsset asset show up in the Content Browser context menu.
bool UGridMapDataFactoryNew::ShouldShowInNewMenu() const
{
	return true;
}

FText UGridMapDataFactoryNew::GetDisplayName() const
{
	//return Super::GetDisplayName();
	return FText::FromString("Grid Map Data");
}

FText UGridMapDataFactoryNew::GetToolTip() const
{
	return FText::FromString("Dn2GameGrids' Map layout asset.");
}


