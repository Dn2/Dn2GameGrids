// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoxGridActor.h"
#include "HexGridActor.generated.h"

/**
 * 
 */
UCLASS()
class DN2GAMEGRIDS_API AHexGridActor : public ABoxGridActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AHexGridActor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid|Hex")
	EHexOffsetMethod HexOffsetMethod;
	
	virtual TArray<FCellInfo> CreateEmptyGrid(int32 XExtent, int32 YExtent, FGameplayTagContainer DefaultTags) override;

	virtual FVector GetCellLocationFromAddress(FCellAddress Address, bool bLocalSpace=false) override;

	virtual FCellAddress GetCellAddressFromLocation(FVector Location) override;

	virtual TArray<FVector> GetCellVertexArray(FCellAddress InAddress, bool bLocalSpace=true) override;
};
