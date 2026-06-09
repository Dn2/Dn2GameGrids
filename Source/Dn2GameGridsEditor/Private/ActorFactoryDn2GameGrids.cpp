// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorFactoryDn2GameGrids.h"
#include "GridActorBase.h"
#include "GridMapData.h"


UActorFactoryDn2GameGrids::UActorFactoryDn2GameGrids() : Super()
{
	DisplayName = FText::FromString("Game Grid Data");
}

bool UActorFactoryDn2GameGrids::CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg)
{
	if (AssetData.IsValid() && AssetData.GetAsset())
	{
		const UGridMapData* GDataAsset = Cast<UGridMapData>(AssetData.GetAsset());
		if (GDataAsset && !GDataAsset->MapClass.IsEmpty() || !GDataAsset->MapClass.Equals("None") && GDataAsset->Extents.X > 1 && GDataAsset->Extents.Y > 1 )
		{
			//UE_LOG(LogTemp, Warning, TEXT("GetSavedClass is %s"), ( GDataAsset->GetSavedClass() ? TEXT("NOT NULL"): TEXT("NULL") ));
			if (GDataAsset->GetSavedClass())
				return true;
		}
	}
	
	return false;
	//return Super::CanCreateActorFrom(AssetData, OutErrorMsg);
}

AActor* UActorFactoryDn2GameGrids::GetDefaultActor(const FAssetData& AssetData)
{
	if (AssetData.IsValid() && AssetData.GetAsset())
	{
		const UGridMapData* GDataAsset = Cast<UGridMapData>(AssetData.GetAsset());
		if (GDataAsset)
		{
			UClass* DefaultActorClass = GDataAsset->GetSavedClass();
			return (DefaultActorClass ? DefaultActorClass->GetDefaultObject<AActor>() : nullptr);
		}
	}
	
	return nullptr;
}


UObject* UActorFactoryDn2GameGrids::GetAssetFromActorInstance(AActor* ActorInstance)
{
	if (ActorInstance)
	{
		AGridActorBase* GridActor = Cast<AGridActorBase>(ActorInstance);
		if (GridActor && GridActor->MapData)
		{
			return GridActor->MapData;
		}
	}

	return nullptr;
}

void UActorFactoryDn2GameGrids::PostSpawnActor(UObject* Asset, AActor* NewActor)
{
	UGridMapData* GDataAsset = Cast<UGridMapData>(Asset);
	AGridActorBase* GridActor = Cast<AGridActorBase>(NewActor);
	if (GDataAsset && GridActor)
	{
		GridActor->MapData = GDataAsset;
		GDataAsset->OnChanged.AddUObject(GridActor, &AGridActorBase::OnGridMapDataChanged);
	}
}

#if ENGINE_MAJOR_VERSION == 4
void UActorFactoryDn2GameGrids::PostCreateBlueprint(UObject* Asset, AActor* CDO)
{
	UGridMapData* GDataAsset = Cast<UGridMapData>(Asset);
	AGridActorBase* GridActor = Cast<AGridActorBase>(CDO);
	if (GDataAsset && GridActor)
	{
		GridActor->MapData = GDataAsset;
		GDataAsset->OnChanged.AddUObject(GridActor, &AGridActorBase::OnGridMapDataChanged);
	}
	
	//Super::PostCreateBlueprint(Asset, CDO);
}
#endif
