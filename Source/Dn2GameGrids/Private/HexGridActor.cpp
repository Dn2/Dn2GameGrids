/*
 * A Hexagon will be defined as https://www.redblobgames.com/grids/hexagons/#basics
 * CellSize is our "size" as defined in the above link (Outer circle radius)
 *
 * 0.8660255f - this is the magic number. Calculated it myself but idk.
 */


#include "HexGridActor.h"

AHexGridActor::AHexGridActor() : Super()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = false;

	HexOffsetMethod = EHexOffsetMethod::HOM_RowOdd;
}

float AHexGridActor::GetCellSize()
{
	return CellSize*0.5f;
}

TArray<FCellInfo> AHexGridActor::CreateEmptyGrid(int32 XExtent, int32 YExtent, FGameplayTagContainer DefaultTags)
{
	return Super::CreateEmptyGrid(XExtent, YExtent, DefaultTags);
}


FVector AHexGridActor::GetCellLocationFromAddress(FCellAddress Address, bool bLocalSpace)
{
	//Actor's origin will be 0,0 of our grid
	FVector Loc = (bLocalSpace ? FVector(0,0,0) : GetActorLocation());

	//float Width = 0.0f;
	float Horiz = 0.0f;
	float Vert = 0.0f;

	float Y=0;
	float X=0;

	if (HexOffsetMethod == EHexOffsetMethod::HOM_RowOdd || HexOffsetMethod == EHexOffsetMethod::HOM_RowEven)
	{
		Horiz = FMath::Sqrt(3.0f)*(GetCellSize());
		Vert = (FMath::Sqrt(3.0f)*(GetCellSize()))*0.8660255f;
		
		Y = Loc.Y + (Address.X + 1) * Horiz - Horiz;
        X = Loc.X - ((Address.Y + 1) * Vert - Vert);

		UE_LOG(LogTemp, Warning, TEXT("Row(Horiz: %f, Vert: %f"), Horiz, Vert);
	}
	else
	{
		Horiz = (FMath::Sqrt(3.0f)*(GetCellSize()))*0.8660255f;
		Vert = (FMath::Sqrt(3.0f)*(GetCellSize()));
		
		Y = Loc.Y + (Address.X + 1) * Horiz;
		X = Loc.X - ((Address.Y + 1) * Vert);

		UE_LOG(LogTemp, Warning, TEXT("Col(Horiz: %f, Vert: %f"), Horiz, Vert);
	}

	
	switch (HexOffsetMethod)
	{
		case EHexOffsetMethod::HOM_RowOdd:
		if (Address.Y % 2)
		{
			Y = Y + (Horiz*0.5f);
		}
		break;

		case EHexOffsetMethod::HOM_RowEven:
		if (!(Address.Y % 2))
		{
			Y = Y + (Horiz*0.5f);
		}
		break;

		case EHexOffsetMethod::HOM_ColOdd:
		if (Address.X % 2)
		{
			X = X + (Vert*0.5f);
		}
		break;
		
		case EHexOffsetMethod::HOM_ColEven:
		if (!(Address.X % 2))
		{
			X = X + (Vert*0.5f);
		}
		break;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Address: %s | Loc: %f, %f"), *Address.ToString(), X, Y);

	return FVector(X, Y, Loc.Z);
}


FCellAddress AHexGridActor::GetCellAddressFromLocation(FVector Location)
{
	FVector ActLoc = GetActorLocation();

	float Horiz = 0.0f;
	float Vert = 0.0f;

	int OffsetCheck = 0;
	
	float X = 0.0f;
	float Y = 0.0f;
	FCellAddress Address = FCellAddress(-1,-1);


	switch (HexOffsetMethod)
	{
		case EHexOffsetMethod::HOM_RowOdd:
		Horiz = FMath::Sqrt(3.0f)*(GetCellSize());
		Vert = (FMath::Sqrt(3.0f)*(GetCellSize()))*0.8660255f;
		
		X = FMath::Abs(((Location.Y - ActLoc.Y) / (Horiz))+0.5f);
		Y = FMath::Abs(((Location.X - ActLoc.X) / (Vert))-0.5f);
		Address.X = FMath::TruncToInt(X);
		Address.Y = FMath::TruncToInt(Y);

		if ((Address.Y % 2))
		{
			X = X - 0.5f;
		}
		
		//UE_LOG(LogTemp, Warning, TEXT("Value Y: %f"), ((Location.Y - ActLoc.Y) / (Horiz))+0.5f);
		UE_LOG(LogTemp, Warning, TEXT("Value X: %f"), ((Location.X - ActLoc.X) / (Vert))-0.5f);
		
		break;

		case EHexOffsetMethod::HOM_RowEven:
		if (!(Address.Y % 2))
		{
			Y = Y + 0.5f;
		}
		break;

		case EHexOffsetMethod::HOM_ColOdd:
		if (Address.X % 2)
		{
			//X = X + (Vert*0.5f);
		}
		break;
		
		case EHexOffsetMethod::HOM_ColEven:
		if (!(Address.X % 2))
		{
			//X = X + (Vert*0.5f);
		}
		break;
	}

	return FCellAddress(FMath::TruncToInt(X),FMath::TruncToInt(Y));
	//return Super::GetCellAddressFromLocation(Location);
}

TArray<FVector> AHexGridActor::GetCellVertexArray(FCellAddress InAddress, bool bLocalSpace)
{
	/*
	 * https://www.redblobgames.com/grids/hexagons/#angles
	 */
	TArray<FVector> Vertices;	
	FVector Loc = GetCellLocationFromAddress(InAddress, bLocalSpace);
	float HexSize = (GetCellSize())/**0.8660255f*/;

	
	for (int i = 0; i < 6; i++)
	{
       	float AngleDeg = 60.0f * i/* - 30.0f*/;
		if (HexOffsetMethod == EHexOffsetMethod::HOM_ColOdd || HexOffsetMethod == EHexOffsetMethod::HOM_ColEven)
		{	AngleDeg-=30.0f;	}
		
		float AngleRad = UE_PI / 180.0f * AngleDeg;
		
		Vertices.Add( FVector(Loc.X + HexSize * FMath::Cos(AngleRad), Loc.Y + HexSize * FMath::Sin(AngleRad), Loc.Z) );
	}
	
	return Vertices;
}
