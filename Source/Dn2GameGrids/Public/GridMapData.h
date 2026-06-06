// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "GridActorBase.h"
#include "GameplayTagContainer.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GridMapData.generated.h"

//class AGridActorBase;

DECLARE_EVENT( UGridMapData, FOnChanged );
/**
 * 
 */
UCLASS()
class DN2GAMEGRIDS_API UGridMapData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UGridMapData();

#if WITH_EDITOR
	virtual void PostEditChangeProperty( FPropertyChangedEvent & PropertyChangedEvent ) override;
#endif
	
	FOnChanged OnChanged;
	
#if WITH_EDITORONLY_DATA
	virtual void Serialize(FStructuredArchive::FRecord Record) override;
#endif
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Grid)
	FString MapClass;
	//TSubclassOf<AGridActorBase> MapClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Grid)
	FIntPoint Extents;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Grid)
	float CellSize;
	
	/*
	*	Desc
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Grid)
	TSet<int32> BlockedCells;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Grid)
	TMap<int32,FGameplayTagContainer> CellTags;

	
	//
	UClass* GetSavedClass() const;
};
