// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGridActor.h"

AHexGridActor::AHexGridActor() : Super()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = false;

	HexOffsetMethod = EHexOffsetMethod::HOM_RowOdd;
}

TArray<FCellInfo> AHexGridActor::CreateEmptyGrid(int32 XExtent, int32 YExtent, FGameplayTagContainer DefaultTags)
{
	return Super::CreateEmptyGrid(XExtent, YExtent, DefaultTags);
}


FVector AHexGridActor::GetCellLocationFromAddress(FCellAddress Address, bool bLocalSpace)
{
	FVector CellLoc = Super::GetCellLocationFromAddress(Address, bLocalSpace);
	int32 X = Address.X;
	int32 Y = Address.Y;
	
	switch (HexOffsetMethod)
	{
		case EHexOffsetMethod::HOM_RowOdd:
			if (Y % 2)
			{
				CellLoc.Y = CellLoc.Y + (CellSize/2);
			}
		break;
		
		case EHexOffsetMethod::HOM_RowEven:
		if (!(Y % 2))
		{
			CellLoc.Y = CellLoc.Y + (CellSize/2);
		}
		break;

		case EHexOffsetMethod::HOM_ColOdd:
		if (X % 2)
		{
			CellLoc.X = CellLoc.X + (CellSize/2);
		}
		break;
		
		case EHexOffsetMethod::HOM_ColEven:
		if (!(X % 2))
		{
			CellLoc.X = CellLoc.X + (CellSize/2);
		}
		break;
	}
	
	return CellLoc;
}


FCellAddress AHexGridActor::GetCellAddressFromLocation(FVector Location)
{
	return Super::GetCellAddressFromLocation(Location);
}

TArray<FVector> AHexGridActor::GetCellVertexArray(FCellAddress InAddress, bool bLocalSpace)
{
	return Super::GetCellVertexArray(InAddress, bLocalSpace);
}
