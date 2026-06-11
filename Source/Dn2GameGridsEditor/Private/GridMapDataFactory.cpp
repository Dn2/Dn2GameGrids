// Fill out your copyright notice in the Description page of Project Settings.


#include "GridMapDataFactory.h"
#include "GridMapData.h"
#include  "JsonUtilities/Public/JsonUtilities.h"

UGridMapDataFactory::UGridMapDataFactory(const FObjectInitializer& ObjectInitializer)
{
	SupportedClass = UGridMapData::StaticClass();
	
	Formats.Add(FString("json;RPG Map 2"));
	Formats.Add(FString("dgm;Dn2 Game Grids Map"));
	
	bCreateNew = false;
	bEditorImport = true;
	
	
}

UObject* UGridMapDataFactory::FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
	UGridMapData* GMD = nullptr;
	FString TextString;
	
	UE_LOG(LogTemp, Warning, TEXT("Imported Filename: %s"), *Filename);
	if (FFileHelper::LoadFileToString(TextString, *Filename))
	{
		FJsonObjectWrapper Obj;
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
				Value->AsString().ParseIntoArray(StringArray, TEXT(":"), true);
				if (StringArray.IsValidIndex(0) && GMD)
				{
					GMD->BlockedCells.Add(FCString::Atoi(*StringArray[0]));
				}
			}
		}
	}
	//GMD->Extents = FIntPoint(16, 16);
	bOutOperationCanceled = false;
	return GMD;
	
	//return Super::FactoryCreateFile(InClass, InParent, InName, Flags, Filename, Parms, Warn, bOutOperationCanceled);
}
