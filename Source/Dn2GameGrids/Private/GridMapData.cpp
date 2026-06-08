// Fill out your copyright notice in the Description page of Project Settings.


#include "GridMapData.h"

class ABoxGridActor;

UGridMapData::UGridMapData()
{
	MapClass = "None";
	Extents = FIntPoint(8, 8);
	CellSize = 200.0f;
}/**/


#if WITH_EDITOR

void UGridMapData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	OnChanged.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("PostEditChangeProperty from GridMapData (OnChanged.Broadcast)!!"));
}

void UGridMapData::BindOnChanged(AGridActorBase* GridActor)
{
	if (GridActor)
	{
		GridActor->MapChangedHandle = OnChanged.AddUObject(GridActor, &AGridActorBase::OnGridMapDataChanged);
	}
}
#endif

#if WITH_EDITORONLY_DATA
void UGridMapData::Serialize(FStructuredArchive::FRecord Record)
{
	Super::Serialize(Record);
	//UE_LOG(LogTemp, Warning, TEXT("GridMapData Updated From Serialize!!"));
}
#endif

UClass* UGridMapData::GetSavedClass() const
{
	if (UClass* SavedClass = FindObject<UClass>(ANY_PACKAGE, *MapClass, true))
	{
		return SavedClass;
	}
	
	if (UObjectRedirector* RenamedClassRedirector = FindObject<UObjectRedirector>(ANY_PACKAGE, *MapClass, true))
	{
		return CastChecked<UClass>(RenamedClassRedirector);
	}
	
#if WITH_EDITOR
	if (UClass* Result = StaticLoadClass(UObject::StaticClass(), nullptr, *MapClass, nullptr, LOAD_None, nullptr))
	{
		return Result;
	}
#endif
	
	return nullptr;
}
