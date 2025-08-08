# Dn2GameGrid

Unreal Engine 4 plug-in for square and hex grids and simple A*(Star) pathfinding. An instance of a grid is managed by an Actor that holds an array of the cells (points/nodes) on the grid. There are helper functions in both C++ and Blueprints for you to manipulate the grid, it's cells and your gameplay actors. Made and tested in Unreal Engine 4.27.

![A cube navigating around walls on a 5 by 5 square grid.](https://img.itch.zone/aW1nLzE2MjQ4Mjc4LmdpZg==/347x500/zPuTuT.gif) <!-- ![A Yellow trail navigating through around walls](https://dn2.github.io/resources/img/astargrid_snek.gif) -->

## Features
- [x] Async grid generation
- [x] Async A* pathfinding
- [x] Non iterative cell validation
- [x] World Location <-> Grid coordinates conversion
- [x] GameplayTag based filtering for cell retrieval
- [x] Cell and Cell Vertex location data
- [x] Grid Movement Component

## Not Implemented (Yet)
- [ ] Hexagon version
- [ ] Common helper cell selection operations (think trace and overlap but for cells, sorta)
- [ ] WFC (Wave Function Collapse) base class
- [ ] Map importing

## Setup
1. Install plugin to your project's plugin folder and enable it by going to Edit > Plugins. e.g: `MyProject\Plugins\Dn2GameGrids`
2. Add a BoxGridActor to your level or create a blueprint with BoxGridActor as its parent class.
3. In the details panel of your placed BoxGridActor  or class defaults panel of your blueprint, under the Grid category, check bAutoGenerate, and set the extents or your grid.
4. Alternately, you can skip step 3 and use the blueprint node `Update Grid Async` to build your base grid at runtime. `Update Grid Async` overwrites the current grid if there is one.
5. Additionally `Update Grid Async`, on completion fires off Blueprint and C++ events with the resulting grid array, grid extents and cell size.

## Terminology
- **Cell**: A point or node on the grid with associated 2D coordinates (e.g 4,20). See `Cell Info`.

- **Cell Address** or **Address**: the 2D coordinates (e.g 7,11) that represent a position on the grid.

- **Cell Info**: `FCellInfo` Struct that holds the info for the cells on a grid. `UpdateGridAsync()` populates `TArray<FCellInfo> GridArray` with these. You can find `FCellAdress`, `FGameplayTagContainer`, etc held within.

- **Cell Tags**: `FGameplayTagContainer` that holds the `FGameTags` that are associated with a `Cell Info`.

- **Grid Extents**: The extents of the grid. A `FIntPoint`, X being how many cells wide and Y being how many cells 'tall'.

- **Neighbors**: Any cells directly next to a given cell. e.g. `GetCellNeighbors()` returns cells next to a given cell.

- **Cell Size**: The size of a cell when calculating its dimensions & location in world space in relation to the `AGridActorBase` that owns it.

## Types
| Name | Members | Description |
|-|-|-|
| FCellAddress | `int32 X` `in32 Y` | Coordinates for a cell, X and Y|
| FCellInfo | `FCellAddress Address` `FGameplayTagContainer CellTags`| Used for grid array. Holds info for each cell in the grid array |
| FAStarCellInfo | `float H` `float G` `float F` `FCellAddress Address` `FCellAddress CameFrom` | Used for the A-Star algo |
| FAStarSearchResults | `bool bFoundGoal` `TArray<FCellAddress> Path` | Struct holding the results of AStar search algorithm |


## AGridActorBase Members
<!--
**`bool` `bAutoGenerate`**: If the actor should generate a grid.
**`float` `CellSize`**: Size of cells.
**`FGameplayTagContainer` `DefaultCellTags`**: If `bAutoGenerate` is true we generate a grid on BeginPlay using these Tags.
**`TArray<FCellInfo>` `GridArray`**: Array holding our grid.
**`UStaticMeshComponent*` `GridMeshComp`**: Mesh component for visuals of the grid and trace collisions for world locations <> grid address conversions.
**`UMaterial*` `GridMat`**:	The material we'll make and instance of and use for visual debugging. -->
| Name                        | Type                  | Description                                                                              |
|-----------------------------|-----------------------|------------------------------------------------------------------------------------------|
| **bAutoGenerate** | **`bool`** | Show a grid preview in editor & generate on BeginPlay |
| **CellSize** | **`float`** | Size of cells |
| **DefaultCellTags** | **`FGameplayTagContainer`** | If `bAutoGenerate`, generate a grid on BeginPlay using these Tags |
| **GridArray** | **`TArray<FCellInfo>`** | Array holding our grid cells |
| **GridMeshComp** | **`UStaticMeshComponent*`** | Visual of the grid and trace collisions for world locations <-> grid address conversions |
| **GridMat** | **`UMaterial*`** | The material we'll make and instance of and use for visual debugging |


## AGridActorBase C++ Functions & Blueprint Nodes
| Name | CPP | BP | Description |
|-|-|-|-|
| **UpdateGridAsync** | Yes | Yes | Create default grid for this grid actor. Populates `TArray<FCellInfo> GridArray`. On completion calls C++ event `ABoxGridActor::OnUpdateGrid_Internal` and blueprint event `ABoxGridActor::OnUpdateGrid` with the results of the grid creation. |
| **GetPathToGoalAsync** | Yes | Yes |Create default grid for this grid actor. Populates `TArray<FCellInfo> GridArray`. On completion calls C++ event `ABoxGridActor::OnUpdateGrid_Internal` and blueprint event `ABoxGridActor::OnUpdateGrid` with the results of the grid creation|
| **CreateEmptyGrid** | Yes | Yes | Create a grid array that you can do what you want with. Not Async. No sort of setups of anything. Just returns an array of `FCellInfo` |
| **AStarSearchToGoal** | Yes | Yes | Not Async. Try to find a path to the goal given addresses and optional filters, using an AStar pathfinding algorithm |
| **GetCellLocationFromAddress**| Yes | Yes | Given the address of a cell (e.g 12,24) return the cell's world location. Uses the Actor's GridArray |
| **GetCellAddressFromLocation** | Yes | Yes | Given a world location (e.g `FVector(123,456,10)`) return a cell address IF the address is valid. Return value of `FCellAddress(-1,-1)` means there is no cell associated with given location. Uses the Actor's GridArray |
| **GetIndexFromAddress** | Yes | Yes | Given the address of a cell, return the Index for that cell's `FCellInfo` in `AGridActorBase::GridArray`. Avoids iterating arrays by just calculation its index |
| **DoesCellExist** | Yes | Yes | Returns `true` if the given address is valid and the `FCellInfo` in the assumed index of `AGridActorBase::GridArray` matches |
| **GetCellInfoByAddress** | Yes | Yes | Given and address returns the associated `FCellInfo` from `AGridActorBase::GridArray`. Make sure the returned `FCellInfo::Address` is valid. i.e. not `FCellAddress(-1,-1)` |
| **GetCellNeighbors** | Yes | Yes | Given an address, an array of neighbouring cells are returned. Optionally filtered using GameplayTags. ToDo: maybe return addresses instead of cell infos |
| **GetGridExtents** | Yes | Yes | Returns the extents of the grid. |


<!--
**`AGridActorBase::UpdateGridAsync()`**: Create default grid for this grid actor. Populates `TArray<FCellInfo> GridArray`. On completion calls C++ event `ABoxGridActor::OnUpdateGrid_Internal` and blueprint event `ABoxGridActor::OnUpdateGrid` with the results of the grid creation.

**`AGridActorBase::GetPathToGoalAsync()`**: Try to find a path to the goal given addresses and optional filters, using an AStar pathfinding algorithm. On completion calls C++ event `ABoxGridActor::OnAStarSearchEnd_Internal` and blueprint event `ABoxGridActor::OnAStarSearchEnd` with the results of the search.

**`AGridActorBase::CreateEmptyGrid()`**: Create a grid array that you can do what you want with. Not Async. No sort of setups of anything. Just an array of `FCellInfo`s.

**`AGridActorBase::AStarSearchToGoal()`**: Not Async. Try to find a path to the goal given addresses and optional filters, using an AStar pathfinding algorithm. (this version SHOULD take a FCellInfo array as a praram. Will fix soon).

**`AGridActorBase::GetCellLocationFromAddress()`**: Given the address of a cell (e.g 12,24) return the cell's world location. Uses the Actor's GridArray;

**`AGridActorBase::GetCellAddressFromLocation()`**: Given a world location (e.g `FVector(123,456,10)`) return a cell address IF the address is valid. Return value of `FCellAddress(-1,-1)` means there is no cell associated with given location. Uses the Actor's GridArray;

**`AGridActorBase::GetIndexFromAddress()`**: Given the address of a cell, return the Index for that cell's `FCellInfo` in `AGridActorBase::GridArray`. Avoids iterating arrays by just calculation its index.

**`AGridActorBase::DoesCellExist()`**: Returns `true` if the given address is valid and the `FCellInfo` in the assumed index of `AGridActorBase::GridArray` matches.

**`AGridActorBase::GetCellInfoByAddress()`**: Given and address returns the associated `FCellInfo` from `AGridActorBase::GridArray`. Make sure the returned `FCellInfo::Address` is valid. i.e. not `FCellAddress(-1,-1)`

**`AGridActorBase::GetCellNeighbors()`**: Given an address, an array of neighbouring cells are returned. Optionally filtered using GameplayTags. ToDo: add FCellInfo array parameter instead of targeting AGridActorBase::GridArray & maybe return addresses instead of cell infos.

**`AGridActorBase::GetGridExtents()`**: Returns the extents of the grid.
-->