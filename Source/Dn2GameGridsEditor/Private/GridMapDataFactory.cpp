// Fill out your copyright notice in the Description page of Project Settings.


#include "GridMapDataFactory.h"
#include "GridMapData.h"
#include  "JsonUtilities/Public/JsonUtilities.h"

UGridMapDataFactory::UGridMapDataFactory(const FObjectInitializer& ObjectInitializer)
{
	SupportedClass = UGridMapData::StaticClass();
	
	Formats.Add(FString("json;RPG Map 2"));
	Formats.Add(FString("dggm;Json Dn2 Game Grids Map"));
	Formats.Add(FString("dgm;Binary Dn2 Game Grids Map"));
	
	bCreateNew = false;
	bEditorImport = true;
	
}

UObject* UGridMapDataFactory::FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
	UGridMapData* GMD = nullptr;
	FString TextString;
	
	UE_LOG(LogTemp, Warning, TEXT("Imported Filename: %s"), *Filename);
	
	if (FPaths::GetExtension(Filename) == "dggm")
	{
		if (FFileHelper::LoadFileToString(TextString, *Filename))
		{
			FJsonObjectWrapper Obj;
			
			GMD = NewObject<UGridMapData>(InParent, InClass, InName, Flags);
            Obj.JsonObjectFromString(TextString);
			
			
			
			// contains any levels?
			if (Obj.JsonObject->HasTypedField<EJson::Array>("levels") && Obj.JsonObject->GetArrayField("levels").Num() > 0 )
			{
				TArray<TSharedPtr<FJsonValue>> Levels = Obj.JsonObject->GetArrayField("levels");

				const TArray<TSharedPtr<FJsonValue>>* Blocked;
				if (Levels[0]->AsObject()->TryGetArrayField("blockedCells",Blocked))
				{
					//extents
					int32 h = 0;
					int32 w = 0;
					if (Levels[0]->AsObject()->TryGetNumberField("h",h) && Levels[0]->AsObject()->TryGetNumberField("w",w) && GMD)
					{
						GMD->Extents = FIntPoint(w, h);
					}
					
					//blocked cells
					for (TSharedPtr<FJsonValue> Value : *Blocked)
					{
						GMD->BlockedCells.Add(Value->AsNumber());
						//UE_LOG(LogTemp, Warning, TEXT("Imported Block: %f"), Value->AsNumber());
					}
					
					//layers
					/*const TArray<TSharedPtr<FJsonValue>>* Layers;
					if (Levels[0]->AsObject()->TryGetArrayField("layers",Layers))
					{
						// for every layer
						for (TSharedPtr<FJsonValue> layerValue : *Layers)
						{
							FString LayerName;
							layerValue->AsObject()->TryGetStringField("name", LayerName);
							
							UE_LOG(LogTemp, Warning, TEXT("Imported Layer Name: %s"), *LayerName);
							
							// if layer name is a valid gametag we add it to every index in the layerDate array
							if (FGameplayTag::IsValidGameplayTagString(LayerName))
							{
								const TSharedPtr<FJsonObject>* layerData;
								if (layerValue->AsObject()->TryGetObjectField("layerData", layerData))
								{
									for (const TTuple<FString, TSharedPtr<FJsonValue>> IndexedCell : layerData->Get()->Values)
									{
										// KeyValuePair.Key
										// KeyValuePair.Value
										
										int cellIndex = FCString::Atoi(*IndexedCell.Key);
										
										UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Imported, *LayerName)
										
										//if (GMD->CellTags[cellIndex].IsValid() && !GMD->CellTags[cellIndex].HasTag(LayerTag))
										//{
											GMD->CellTags[cellIndex].AddTag(TAG_Imported);
										//}
									}
								}
							}
							
							//check every index value for a valid game tag
						}
					}*/
					//gametags
					
				}
				
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Imported: %s, level's array is empty"), *TextString);
				return nullptr;
			}
		}
	}
	
	// if json;RPG Map 2. should check for valid map header
	if (FPaths::GetExtension(Filename) == "json")
	{
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
	}

	//GMD->Extents = FIntPoint(16, 16);
	bOutOperationCanceled = false;
	return GMD;
	
	//return Super::FactoryCreateFile(InClass, InParent, InName, Flags, Filename, Parms, Warn, bOutOperationCanceled);
}
