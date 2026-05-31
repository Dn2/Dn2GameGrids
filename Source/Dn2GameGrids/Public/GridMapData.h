// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GridMapData.generated.h"

/**
 * 
 */
UCLASS()
class DN2GAMEGRIDS_API UGridMapData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	//UGridMapData();
	 
	UPROPERTY(BlueprintReadWrite, Category = Grid)
	FIntPoint Extents;
	
	/*
	*	Desc
	*/
	UPROPERTY(BlueprintReadWrite, Category = Grid)
	TMap<int32,bool> BlockedCells;
	
	UPROPERTY(BlueprintReadWrite, Category = Grid)
	TMap<int32,FGameplayTagContainer> CellTags;
};
