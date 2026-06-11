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
	
	/*bText = true;
	bEditorImport = true;
	Formats.Add(FString("json;RPG Map 2"));
	Formats.Add(FString("dgm;RPG Map 2"));*/
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

/*UObject* UGridMapDataFactoryNew::FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
	UGridMapData* GMD = nullptr;
	FString TextString;
	FJsonObjectWrapper Obj;
	
	
	UE_LOG(LogTemp, Warning, TEXT("Imported Filename: %s"), *Filename);
	if (FFileHelper::LoadFileToString(TextString, *Filename))
	{
		UE_LOG(LogTemp, Warning, TEXT("Imported: %s"), *TextString);
		
		GMD = NewObject<UGridMapData>(InParent, InClass, InName, Flags);
		Obj.JsonObjectFromString(TextString);
		
		//extents
		int32 h = 0;
		int32 w = 0;
		if (Obj.JsonObject->TryGetNumberField("h",h) && Obj.JsonObject->TryGetNumberField("w",w) && GMD)
		{
			GMD->Extents = FIntPoint(w, h);
		}
		
		
		//collisions
		const TArray<TSharedPtr<FJsonValue>>* Values;
		if (Obj.JsonObject->TryGetArrayField("collisions", Values))
		{
			for (TSharedPtr<FJsonValue> Value : *Values)
			{
				TArray<FString> StringArray;
				Value->AsString().ParseIntoArray(StringArray, TEXT("\""), true);
				if (StringArray.IsValidIndex(0) && GMD)
				{
					GMD->BlockedCells.Add(FCString::Atoi(*StringArray[0]));
				}
			}
		}
	}
	GMD->Extents = FIntPoint(16, 16);
	bOutOperationCanceled = false;
	return GMD;
	//return Super::FactoryCreateFile(InClass, InParent, InName, Flags, Filename, Parms, Warn, bOutOperationCanceled);
}*/

/*UObject* UGridMapDataFactoryNew::FactoryCreateText(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn)
{
	UGridMapData* GMD = nullptr;
	//FString TextString;

	
	
	//UE_LOG(LogTemp, Warning, TEXT("Imported Filename: %s"), *Filename);
	if (Buffer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Imported: %s"), Buffer);
		FJsonObjectWrapper Obj;
		GMD = NewObject<UGridMapData>(InParent, InClass, InName, Flags);
		Obj.JsonObjectFromString(Buffer);
		
		//extents
		int32 h = 0;
		int32 w = 0;
		if (Obj.JsonObject->TryGetNumberField("h",h) && Obj.JsonObject->TryGetNumberField("w",w) && GMD)
		{
			GMD->Extents = FIntPoint(w, h);
		}
		
		
		//collisions
		const TArray<TSharedPtr<FJsonValue>>* Values;
		if (Obj.JsonObject->TryGetArrayField("collisions", Values))
		{
			for (TSharedPtr<FJsonValue> Value : *Values)
			{
				TArray<FString> StringArray;
				Value->AsString().ParseIntoArray(StringArray, TEXT("\""), true);
				if (StringArray.IsValidIndex(0) && GMD)
				{
					GMD->BlockedCells.Add(FCString::Atoi(*StringArray[0]));
				}
			}
		}
	}
	//GMD->Extents = FIntPoint(16, 16);
	//bOutOperationCanceled = false;
	return GMD;
	
	//return Super::FactoryCreateText(InClass, InParent, InName, Flags, Context, Type, Buffer, BufferEnd, Warn);
}*/

FText UGridMapDataFactoryNew::GetDisplayName() const
{
	//return Super::GetDisplayName();
	return FText::FromString("Grid Map Data");
}

FText UGridMapDataFactoryNew::GetToolTip() const
{
	return FText::FromString("Dn2GameGrids' Map layout asset.");
}


