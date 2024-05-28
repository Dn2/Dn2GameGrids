// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Dn2GameGridTypes.h"
#include "Dn2GameGridsBFL.generated.h"

/**
 * 
 */
UCLASS()
class DN2GAMEGRIDS_API UDn2GameGridsBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	//To string
	UFUNCTION(BlueprintPure, meta = (DisplayName = "To String", CompactNodeTitle = "->", BlueprintAutocast), Category = GameGrid)
	static FString Conv_FCellAddressToString(FCellAddress Struct);


	/** Returns true if CellAddress A is equal to CellAddress B (A == B) */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (CellAddress)", CompactNodeTitle = "==", ScriptMethod = "Equals", ScriptOperator = "==", Keywords = "== equal"), Category = "GridGame|Operators")
	static bool Equal_CellAddress(FCellAddress A, FCellAddress B);

	/**  
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Set Blocked (CellInfo)", CompactNodeTitle = "SetBlocked", Keywords = "Block Blocked"), Category = "GridGame|CellInfo")
	static void Set_CellInfoBlocked(FCellAddress Cell, bool Blocked);*/

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To North", CompactNodeTitle = "N", BlueprintAutocast), Category = GameGrid)
	static FCellAddress Conv_GetCellNorth(FCellAddress Address);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To South", CompactNodeTitle = "S", BlueprintAutocast), Category = GameGrid)
	static FCellAddress Conv_GetCellSouth(FCellAddress Address);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To East", CompactNodeTitle = "E", BlueprintAutocast), Category = GameGrid)
	static FCellAddress Conv_GetCellEast(FCellAddress Address);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To West", CompactNodeTitle = "W", BlueprintAutocast), Category = GameGrid)
	static FCellAddress Conv_GetCellWest(FCellAddress Address);
};
