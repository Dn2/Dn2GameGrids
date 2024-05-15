// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayTagsManager.h"
#include "BoxGridActor.h"

// Sets default values
ABoxGridActor::ABoxGridActor() : Super()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	OnUpdateGridDelCPP.AddDynamic(this, &ABoxGridActor::OnUpdateGrid_Internal);
	OnUpdateGridDelBP.AddDynamic(this, &ABoxGridActor::OnUpdateGrid);

	OnAStarSearchEndDelCPP.AddDynamic(this, &ABoxGridActor::OnAStarSearchEnd_Internal);
	OnAStarSearchEndDelBP.AddDynamic(this, &ABoxGridActor::OnAStarSearchEnd);

	//load default grid mesh and material
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GridMeshObj(TEXT("/Dn2GameGrids/StaticMeshes/Gridx100.Gridx100"));
	static ConstructorHelpers::FObjectFinder<UMaterial> GridMatObj(TEXT("/Dn2GameGrids/Materials/GridChecker_Mat.GridChecker_Mat"));
	if (GridMeshComp && GridMeshObj.Object)
	{
		GridMeshComp->SetStaticMesh(GridMeshObj.Object);
	}
	if (GridMeshComp && GridMatObj.Object)
	{
		GridMat = GridMatObj.Object;
		GridMeshComp->SetMaterial(0, GridMat);
	}

	PostUpdateGridSetup(false);
}

// Called when the game starts or when spawned
void ABoxGridActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (bAutoGenerate)
	{
		UE_LOG(LogTemp, Warning, TEXT("Auto-Generating..."));
		//(new FAutoDeleteAsyncTask<FUpdateGridTask>(this, FIntPoint(10000,10000), 100))->StartBackgroundTask();
		UpdateGridAsync(GetGridExtents(), CellSize, DefaultCellTags);
	}
}

// Called every frame
void ABoxGridActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


/*void ABoxGridActor::OnConstruction(const FTransform& Transform)
{

}*/


#if WITH_EDITOR
void ABoxGridActor::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	if (e.GetPropertyName().ToString() == "X"|| "Y" || "DefaultGridExtents" || "CellSize")
	{
		PostUpdateGridSetup();
	}
}
#endif


bool ABoxGridActor::UpdateGridAsync(FIntPoint Extents, float GridCellSize, FGameplayTagContainer DefaultTags)
{
	//if there is an async task related to the grid currently running do nothing and return false
	if (IsBusy())
	{
		return false;
	}

	//else do ya thing (start the async task, create the new grid and update values)
	UE_LOG(LogTemp, Warning, TEXT("Updating Grid..."));
	SetBusy(true);
	(new FAutoDeleteAsyncTask<FUpdateGridTask>(this, Extents, GridCellSize))->StartBackgroundTask();

	return true;
}


bool ABoxGridActor::GetPathToGoalAsync(FCellAddress Start, FCellAddress Goal, FGameplayTagContainer InFilter, FGameplayTagContainer ExFilter, bool bConers)
{
	//if there is an async task related to the grid currently running do nothing and return false
	if (IsBusy())
	{
		return false;
	}

	//else do ya thing (start the async task, search for the goal on the grid and return values)
	UE_LOG(LogTemp, Warning, TEXT("Seaching from %s to %s..."), *Start.ToString(), *Goal.ToString());
	SetBusy(true);
	(new FAutoDeleteAsyncTask<FStarSearchToGoalTask>(this, Start, Goal, InFilter, ExFilter, bConers))->StartBackgroundTask();

	return true;
}

TArray<FCellInfo> ABoxGridActor::CreateEmptyGrid(int32 XExtent, int32 YExtent, FGameplayTagContainer DefaultTags)
{
	TArray<FCellInfo> NewGridArray;

	//if extents are too low, return and empty array
	if (XExtent < 1 && YExtent < 1)
	{
		return NewGridArray;
	}


	/*
	*	Zeroed out indexes for the cell creation loop.
	*	X = rows, Y = columns, Index = the index of the cell in the cell array (CellArray).
	*/
	int32 X = 0;
	int32 Y = 0;
	//int32 Index = 0;

	/*
	*	Creating our grid...
	*/
	while (Y < YExtent)
	{

		while (X < XExtent)
		{
			FCellInfo NewCell = FCellInfo(FCellAddress(X, Y));
			NewCell.CellTags.AppendTags(DefaultTags);
			NewGridArray.Add(NewCell);

			++X;
		}

		X = 0;
		Y++;
	}

	UE_LOG(LogTemp, Warning, TEXT("GridArray Size: %d"), NewGridArray.Num());

	return NewGridArray;
}


void ABoxGridActor::PostUpdateGridSetup(bool bUpdateMaterial)
{
	if (GridMeshComp)
	{
		//Scale mesh (maybe move this to be its own function)
		FVector MeshScale;
		MeshScale.X = (CellSize * 0.01f) * GetGridExtents().Y;
		MeshScale.Y = (CellSize * 0.01f) * GetGridExtents().X;
		MeshScale.Z = 1;
		GridMeshComp->SetWorldScale3D(MeshScale);
		
		MeshScale.X = GetActorLocation().X - (GetGridExtents().Y * CellSize) * 0.5f;
		MeshScale.Y = GetActorLocation().Y + (GetGridExtents().X * CellSize) * 0.5f;
		MeshScale.Z = GetActorLocation().Z;

		GridMeshComp->SetWorldLocation(MeshScale);

		if (bUpdateMaterial)
		{
			if (!GridMatInst && GridMat)
			{
				GridMatInst = UMaterialInstanceDynamic::Create(GridMat, this);
				GridMatInst->SetScalarParameterValue(FName("X"), GetGridExtents().Y);
				GridMatInst->SetScalarParameterValue(FName("Y"), GetGridExtents().X);

				GridMeshComp->SetMaterial(0, GridMatInst);
			}
			else if (GridMatInst)
			{
				GridMatInst->SetScalarParameterValue(FName("X"), GetGridExtents().Y);
				GridMatInst->SetScalarParameterValue(FName("Y"), GetGridExtents().X);
				GridMeshComp->SetMaterial(0, GridMatInst);
			}
		}
	}
}


FVector ABoxGridActor::GetCellLocationFromAddress(FCellAddress Address)
{
	//Actor's origin will be 0,0 of our grid
	FVector Loc = GetActorLocation();

	float Y = Loc.Y + (Address.X + 1) * CellSize - CellSize * 0.5f;
	float X = Loc.X - ((Address.Y + 1) * CellSize - CellSize * 0.5f);

	return FVector(X, Y, Loc.Z);
}

FCellAddress ABoxGridActor::GetCellAddressFromLocation(FVector Location)
{
	/*
		A cell's world location will never be less than the actors origin point or the grid extents max distance
		We check here for validity
	*/
	FVector ActLoc = GetActorLocation();
	if (Location.Y < ActLoc.Y || Location.Y > ActLoc.Y + (GetGridExtents().X*CellSize) || Location.X > ActLoc.X || Location.X < ActLoc.X - (GetGridExtents().Y*CellSize))
	{
		UE_LOG(LogTemp, Warning, TEXT("Exited from 1st check: %s"), (Location.X < CellSize * GetGridExtents().X ? TEXT("X1 true") : TEXT("X1 false")) );
		return FCellAddress(-1, -1);
	}

	FCellAddress Address;
	Address.X = FMath::TruncToFloat(FMath::Abs(Location.Y - ActLoc.Y) / CellSize);
	Address.Y = FMath::TruncToFloat(FMath::Abs(Location.X - ActLoc.X) / CellSize);

	//check for out of range
	if (Address.X < 0 || Address.Y < 0 || Address.X > GetGridExtents().X-1 || Address.Y > GetGridExtents().Y-1)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Failed at Address range check: %s"), *Address.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Exited from 2nd check: %s, %d %d"), (Address.Y > GetGridExtents().Y ? TEXT("AddX true") : TEXT("AddX false")), Address.X, Address.Y);
		return FCellAddress(-1, -1);
	}

	//check if cell exist
	if (DoesCellExist(Address))
	{
		return Address;
	}

	UE_LOG(LogTemp, Warning, TEXT("Exited from 3rd check %s"), *Address.ToString());

	return FCellAddress(-1, -1);
}

int32 ABoxGridActor::GetIndexFromAddress(FCellAddress Address) const
{
	//return (GetGridExtents().Y * Address.X) + Address.Y;
	return Address.Y * GetGridExtents().X + Address.X;
}

/*
	Calculate the index of where the InAddress cell should exist then compare it's address.
	If the addresses match the cell does exist.
*/
bool ABoxGridActor::DoesCellExist(FCellAddress Address) const
{
	if (Address.X < 0 || Address.X > GetGridExtents().X-1 || Address.Y < 0 || Address.Y > GetGridExtents().Y-1)
		return false;

	UE_LOG(LogTemp, Warning, TEXT("A: %s"), *Address.ToString());

	int32 Index = GetIndexFromAddress(Address);

	if (!GridArray.IsValidIndex(Index))
		return false;

	UE_LOG(LogTemp, Warning, TEXT("B: %s"), *Address.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Index: %d | IsValid: %s"), Index, (GridArray.IsValidIndex(Index) ? TEXT("true") : TEXT("false")) );
	UE_LOG(LogTemp, Warning, TEXT("InAddress: %d, %d | IndexAddress: %d, %d | Index: %d"), Address.X, Address.Y, GridArray[Index].Address.X, GridArray[Index].Address.Y, Index);

	return(GridArray.IsValidIndex(Index) && GridArray[Index].Address == Address);
}

/*
	Check if a cell is walkable or not.
	Should ALWAYS call DoesCellExist() before calling this as this does not compare addresses or check if array index is valid

bool ABoxGridActor::IsCellBlocked(FCellAddress Address) const
{
	int32 Index = GetIndexFromAddress(Address);

	return(GridArray[Index].bBlocked);
}*/

/*
	Set if a cell is walkable or not.
	Should ALWAYS call DoesCellExist() before calling this as this does not compare addresses or check if array index is valid

void ABoxGridActor::SetCellBlocked(FCellAddress Address, bool bBlocked)
{
	if (DoesCellExist(Address))
	{
		int32 Index = GetIndexFromAddress(Address);
		GridArray[Index].bBlocked = bBlocked;
	}

}*/

FCellInfo ABoxGridActor::GetCellInfoByAddress(FCellAddress Address) const
{
	if (DoesCellExist(Address))
	{
		int32 Index = GetIndexFromAddress(Address);

		return GridArray[Index];
	}

	return FCellInfo();
}

/*
*	Easy A* (star) Pathfinding by Nicholas Swift
*	https://medium.com/@nicholas.w.swift/easy-a-star-pathfinding-7e6689c7f7b2
* 
*	Introduction to the A* Algorithm by Amit Patel
*	https://www.redblobgames.com/pathfinding/a-star/introduction.html
*/
FAStarSearchResults ABoxGridActor::AStarSearchToGoal(FCellAddress Start, FCellAddress Goal, FGameplayTagContainer InFilters, FGameplayTagContainer ExFilters, bool bCorners)
{
	if (!DoesCellExist(Start) || !DoesCellExist(Goal))
	{
		UE_LOG(LogTemp, Warning, TEXT("Found NOT FOUND. Early exit"));
		return FAStarSearchResults();
	}
	

	// Initialize both open and closed list
	TArray<FAStarCellInfo> OpenList;
	TArray<FAStarCellInfo> ClosedList;
	TMap<FCellAddress, FCellAddress> CameFrom;

	// Add the start node
	FAStarCellInfo StartNode = FAStarCellInfo(Start, FCellAddress(-1, -1));
	StartNode.G = 0;
	StartNode.F = StartNode.G + FMath::Max(FMath::Abs(Start.X - Goal.X), FMath::Abs(Start.Y - Goal.Y));

	OpenList.Add(StartNode);

	// Loop until you find the end
	while (OpenList.Num() > 0)
	{
		// Get the current node
		//let the currentNode equal the node with the least f value
		FAStarCellInfo CurrentNode = OpenList[0];

		for (FAStarCellInfo node : OpenList)
		{
			if (node.F < CurrentNode.F)
			{
				CurrentNode = node;
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("Search: While Open List"));
		// Found the goal
		//if currentNode is the goal
		if (CurrentNode.Address == Goal)
		{
			FAStarSearchResults results;
			results.bFoundGoal = true;

			FAStarCellInfo current = CurrentNode;

			while (current.CameFrom != FCellAddress(-1, -1))
			{
				results.Path.Add(current.Address);
				current = *ClosedList.FindByPredicate([&](FAStarCellInfo& InItem)
				{
					return InItem.Address == current.CameFrom;
				});
			}

			UE_LOG(LogTemp, Warning, TEXT("Found goal. Count: %d"), results.Path.Num());
			Algo::Reverse(results.Path);
			return results;
		}


		//remove the currentNode from the openList
		for (int i = 0; i < OpenList.Num(); i++)
		{
			if (OpenList[i].Address == CurrentNode.Address)
			{
				OpenList.RemoveAt(i);
			}

		}

		//add the currentNode to the closedList
		ClosedList.Add(CurrentNode);


		// Generate children
		//let the children of the currentNode equal the adjacent nodes
		TArray<FCellInfo> ChildrenNodes = GetCellNeighbors(CurrentNode.Address, InFilters, ExFilters, bCorners);

		for (FCellInfo cell : ChildrenNodes)
		{
			// Child is NOT on the closedList
			FAStarCellInfo ChildCell = FAStarCellInfo(cell.Address, CurrentNode.Address);

			if (!ClosedList.Contains(ChildCell) && !IsCellBlocked(ChildCell.Address)/**/)
			{
				// Create the f, g, and h values
				ChildCell.G = CurrentNode.G + 1;
				ChildCell.H = FMath::Max(FMath::Abs(ChildCell.Address.X - Goal.X), FMath::Abs(ChildCell.Address.Y - Goal.Y));
				ChildCell.F = ChildCell.G + ChildCell.H;

				bool AddToOpen = true;
				//Child is NOT already in the open list or g
				for (FAStarCellInfo OpenNode : OpenList)
				{
					if (ChildCell == OpenNode && ChildCell.G >= OpenNode.G)
					{
						AddToOpen = false;
						break;
					}
				}

				if (AddToOpen)
				{
					OpenList.Add(ChildCell);
				}
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Found not FOUND: Last exit"));
	return FAStarSearchResults();
}

TArray<FCellInfo> ABoxGridActor::GetCellNeighbors(FCellAddress Address, FGameplayTagContainer InFilters, FGameplayTagContainer ExFilters, bool bCorners /*= true*/) const
{
	TArray<FCellInfo> Neighbours;
	FCellAddress CellAddress;
	//FIntPoint ClampedCellAddress;

	//return empty array if invalid address
	if (!DoesCellExist(Address))
	{
		return Neighbours;
	}

	int32 X = Address.X;
	int32 Y = Address.Y;


	//X +
	CellAddress = FIntPoint(X + 1, Y);
	if (DoesCellExist(CellAddress) && !IsCellBlocked(CellAddress))
		Neighbours.Add(GetCellInfoByAddress(CellAddress));

	//X -
	CellAddress = FIntPoint(X - 1, Y);
	if (DoesCellExist(CellAddress) && !IsCellBlocked(CellAddress))
		Neighbours.Add(GetCellInfoByAddress(CellAddress));

	//Y +
	CellAddress = FIntPoint(X, Y + 1);
	if (DoesCellExist(CellAddress) && !IsCellBlocked(CellAddress))
		Neighbours.Add(GetCellInfoByAddress(CellAddress));

	//Y -
	CellAddress = FIntPoint(X, Y - 1);
	if (DoesCellExist(CellAddress) && !IsCellBlocked(CellAddress))
		Neighbours.Add(GetCellInfoByAddress(CellAddress));

	if (bCorners)
	{
		//X-, Y+
		CellAddress = FIntPoint(X - 1, Y + 1);
		if (DoesCellExist(CellAddress) && !IsCellBlocked(CellAddress))
			Neighbours.Add(GetCellInfoByAddress(CellAddress));

		//X+, Y-
		CellAddress = FIntPoint(X + 1, Y - 1);
		if (DoesCellExist(CellAddress) && !IsCellBlocked(CellAddress))
			Neighbours.Add(GetCellInfoByAddress(CellAddress));

		//X-, Y-
		CellAddress = FIntPoint(X - 1, Y - 1);
		if (DoesCellExist(CellAddress) && !IsCellBlocked(CellAddress))
			Neighbours.Add(GetCellInfoByAddress(CellAddress));

		//X+, Y+
		CellAddress = FIntPoint(X + 1, Y + 1);
		if (DoesCellExist(CellAddress) && !IsCellBlocked(CellAddress))
			Neighbours.Add(GetCellInfoByAddress(CellAddress));
	}

	/* Apply filters */

	//remove cells that don't contain include tags
	if (!InFilters.IsEmpty())
	{
		for (FCellInfo Cell : Neighbours)
		{
			if (!Cell.CellTags.HasAll(InFilters))
			{
				Neighbours.Remove(Cell);
			}
		}
	}

	//remove cells that contain the exclude tags
	if (!ExFilters.IsEmpty())
	{
		for (FCellInfo Cell : Neighbours)
		{
			if (Cell.CellTags.HasAny(ExFilters))
			{
				Neighbours.Remove(Cell);
			}
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Neighbours: %d"), Neighbours.Num());

	return Neighbours;
}

void ABoxGridActor::OnUpdateGrid_Internal(const FIntPoint& OutGridExtents, const TArray<FCellInfo>& OutCellArray, const float OutCellSize)
{
	AsyncTask(ENamedThreads::GameThread, [this, OutGridExtents, OutCellArray, OutCellSize]()
	{
		UE_LOG(LogTemp, Warning, TEXT("Done Updating Grid!"));
		SetBusy(false);

		PostUpdateGridSetup();

		//call blueprint event
		if (OnUpdateGridDelBP.IsBound())
		{
			OnUpdateGridDelBP.Broadcast(OutGridExtents, OutCellArray, OutCellSize);
		}
	});
}

void ABoxGridActor::OnAStarSearchEnd_Internal(const FAStarSearchResults& AStarSearchResults, const bool GoalFound, const int32 NumberOfCells, const FCellAddress& LastCellFound)
{
	AsyncTask(ENamedThreads::GameThread, [this, AStarSearchResults, GoalFound, NumberOfCells, LastCellFound]()
	{
		UE_LOG(LogTemp, Warning, TEXT("Done AStar path finding!!"));
		SetBusy(false);

		//call blueprint event
		if (OnAStarSearchEndDelBP.IsBound())
		{
			OnAStarSearchEndDelBP.Broadcast(AStarSearchResults, GoalFound, NumberOfCells, LastCellFound);
		}
	});
}




