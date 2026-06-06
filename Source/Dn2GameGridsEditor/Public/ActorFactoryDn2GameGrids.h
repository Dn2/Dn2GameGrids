// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorFactories/ActorFactory.h"
#include "ActorFactoryDn2GameGrids.generated.h"

/**
 * 
 */
UCLASS()
class DN2GAMEGRIDSEDITOR_API UActorFactoryDn2GameGrids : public UActorFactory
{
	GENERATED_BODY()
	
	UActorFactoryDn2GameGrids();
	
public:
	virtual bool CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg) override;
	virtual AActor* GetDefaultActor(const FAssetData& AssetData) override;
	virtual UObject* GetAssetFromActorInstance(AActor* ActorInstance) override;
	
protected:
	virtual void PostSpawnActor(UObject* Asset, AActor* NewActor) override;
	virtual void PostCreateBlueprint(UObject* Asset, AActor* CDO) override;
};
