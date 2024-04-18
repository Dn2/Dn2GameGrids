# Dn2GameGrid

An Unreal Engine 4 plug-in for square and hex grids and simple A* pathfinding. An instance of a grid is managed by an Actor that holds an array of the cells (points/nodes) on the grid. There are helper functions in both C++ and Blueprints for you to manipulate the grid, it's cells and your gameplay actors.

## Features
- [x] Async grid population
- [x] Async A* pathfinding
- [x] Non iterative cell validation
- [x] World Location <-> Grid coordinates conversion
- [x] GameplayTag based filtering for cell retrieval

## Not Implemented (Yet)
- [ ] Hexagon version
- [ ] Common helper cell selection operations (think trace and overlap but for cells, sorta)
- [ ] WFC base class

## Setup




## Terminology
- **Cell**: A point or node on the grid with associated 2D coordinates (e.g 4,20). See `Cell Info`.
- **Cell Address** or **`Address`**: the 2D coordinates (e.g 7,11) that represent a position on the grid.
- **Cell Info**: `FCellInfo` Struct that holds the info for the cells on a grid. `UpdateGridAsync()` populates `TArray<FCellInfo> GridArray` with these. You can find `FCellAdress`, `FGameplayTagContainer`, etc held within.
- **`Cell Tags`**: `FGameplayTagContainer` that holds the `FGameTags` that are associated with a `Cell Info`.
- **`Grid Extents`**: The extents of the grid. A `FIntPoint`, X being number if rows and Y being number if column.
- **`Neighbors`**: Any cells directly next to a given cell. e.g. `GetCellNeighborsFromAddress()` returns cells next to a given cell.
- **`Cell Size`**: The size of a cell when calculating its dimensions & location in world space in relation to the `AGridActorBase` that owns it.

## Members
**bool bAutoGenerate**: If the actor should generate a grid.
**`float` `CellSize`**: Size of cells.
**`FGameplayTagContainer` `DefaultCellTags`**: If `bAutoGenerate` is true we generate a grid on BeginPlay using these Tags.
**`TArray<FCellInfo>` `GridArray`**: Array holding our grid.
**`UStaticMeshComponent*` `GridMeshComp`**: Mesh component for visuals of the grid and trace collisions for world locations <> grid address conversions.
**`UMaterial*` `GridMat`**:	The material we'll make and instance of and use for visual debugging.

## C++ Functions
**`AGridActorBase::UpdateGridAsync()`**: Create default grid for this grid actor. Populates `TArray<FCellInfo> GridArray`. On completion calls C++ event `ABoxGridActor::OnUpdateGrid_Internal` and blueprint event `ABoxGridActor::OnUpdateGrid` with the results of the grid creation.

**`AGridActorBase::GetPathToGoalAsync()`**: Try to find a path to the goal given addresses and optional filters, using an AStar pathfinding algorithm. On completion calls C++ event `ABoxGridActor::OnAStarSearchEnd_Internal` and blueprint event `ABoxGridActor::OnAStarSearchEnd` with the results of the search.

**`AGridActorBase::CreateEmptyGrid()`**: Create a grid array that you can do what you want with. Not Async. No sort of setups of anything. Just an array of `FCellInfo`s.

**`AGridActorBase::AStarSearchToGoal()`**: Not Async. Try to find a path to the goal given addresses and optional filters, using an AStar pathfinding algorithm. (this version SHOULD take a FCellInfo array as a praram. Will fix soon).

**`AGridActorBase::GetCellLocationFromAddress()`**: Given the address of a cell (e.g 12,24) return the cell's world location. Uses the Actor's GridArray;

**`AGridActorBase::GetCellAddressFromLocation()`**: Given a world location (e.g `FVector(123,456,10)`) return a cell address IF the address is valid. Return value of `FCellAddress(-1,-1)` means there is no cell associated with given location. Uses the Actor's GridArray;

**`AGridActorBase::GetIndexFromAddress()`**: Given the address of a cell, return the Index for that cell's `FCellInfo` in `AGridActorBase::GridArray`. Avoids iterating arrays by just calculation its index.

**`AGridActorBase::DoesCellExist()`**: Returns `true` if the given address is valid and the `FCellInfo` in the assumed index of `AGridActorBase::GridArray` matches.

**`AGridActorBase::GetCellInfoByAddress()`**: Given and address returns the associated `FCellInfo` from `AGridActorBase::GridArray`. Make sure the returned `FCellInfo::Address` is valid. i.e. not `FCellAddress(-1,-1)`

**`AGridActorBase::GetCellNeighborsFromAddress()`**: Given an address, an array of neighbouring cells are returned. Optionally filtered using GameplayTags. ToDo: add FCellInfo array parameter instead of targeting AGridActorBase::GridArray & maybe return addresses instead of cell infos.

**`AGridActorBase::GetGridExtents()`**: Returns the extents of the grid.



## Blueprint Nodes