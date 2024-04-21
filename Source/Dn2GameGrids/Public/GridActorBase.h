// Fill out your copyright notice in the Description page of Project Settings.
/*
	This is the base class and most comments here will be referring to what the code
	in the subclasses should be doing or using ABoxGridActor as an example. See subclasses
	for actual implementations or subclass this or its subclasses to modify to your liking.
*/
#pragma once

#include "GameplayTags.h"
#include "GameplayTagContainer.h"
#include "Components/BillboardComponent.h"
#include "Dn2GameGridTypes.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridActorBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUpdateGrid_Internal, const FIntPoint&, gridExtents, const TArray<FCellInfo>&, OutCellArray, const float, OutCellSize);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUpdateGrid, const FIntPoint&, gridExtents, const TArray<FCellInfo>&, OutCellArray, const float, OutCellSize);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAStarSearchEnd_Internal, const FAStarSearchResults&, AStarSearchResults, const bool, GoalFound, const int32, NumberOfCells, const FCellAddress&, LastCellFound);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAStarSearchEnd, const FAStarSearchResults&, AStarSearchResults, const bool, GoalFound, const int32, NumberOfCells, const FCellAddress&, LastCellFound);


UCLASS()
class DN2GAMEGRIDS_API AGridActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Set bBusy indicating if this actor is able to run async grid creation or path-finding task at this time
	void SetBusy(bool bIsBusy);

	/*
	*	My dirty way of only letting one async function run at a time so that we aren't trying to
	*	navigate the grid while creating a new one.
	*/
	bool IsBusy();



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY()
	FOnUpdateGrid_Internal OnUpdateGridDelCPP;

	UPROPERTY()
	FOnUpdateGrid OnUpdateGridDelBP;

	UPROPERTY()
	FOnAStarSearchEnd_Internal OnAStarSearchEndDelCPP;

	UPROPERTY()
	FOnAStarSearchEnd OnAStarSearchEndDelBP;


	/*
	*	Where we will call the subclass's version of an async FNonAbandonableTask that updates GridExtents, CellSize,
	*	and calls CreateEmptyGrid() that builds a new array of FCellInfo that will be our grid. Sets bBusy=true
	*	until it is completed.
	*
	*	In ABoxGridActor's case our FNonAbandonableTask is FUpdateGridTask.
	* 
	*	C++ and blueprint events are called on completion, Where you can do whatever.
	*/
	UFUNCTION(BlueprintCallable, Category = Grid, meta = (AutoCreateRefTerm = "DefaultTags"))
	virtual bool UpdateGridAsync(FIntPoint Extents, float GridCellSize, FGameplayTagContainer DefaultTags);

	/*
	*	Where we will call the subclass's version of an FNonAbandonableTask that given a start and goal address will
	*	call a function that will try to find a path using an AStar path-finding algo.
	* 
	*	In ABoxGridActor's case our FNonAbandonableTask is FStarSearchToGoalTask.
	* 
	*	C++ and blueprint events are called on completion, where you can do whatever.
	*/
	UFUNCTION(BlueprintCallable, Category = Grid)
	virtual bool GetPathToGoalAsync(FCellAddress Start, FCellAddress Goal, FGameplayTagContainer InFilter, FGameplayTagContainer ExFilter, bool bConers);

	/*
	*	Where we will create the default array of our cells for the grid. Creates an array of
	*	FCellInfo with their coordinates as FCellAddress.
	*/
	UFUNCTION(BlueprintCallable, Category = Grid, meta = (AutoCreateRefTerm = "DefaultTags"))
	virtual TArray<FCellInfo> CreateEmptyGrid(int32 XExtent, int32 YExtent, FGameplayTagContainer DefaultTags);


	UFUNCTION(BlueprintCallable, Category = Grid)
	virtual FVector GetCellLocationFromAddress(FCellAddress Address);

	UFUNCTION(BlueprintCallable, Category = Grid)
	virtual FCellAddress GetCellAddressFromLocation(FVector Location);

	//Convert Address to Index, where a cell should be in and array of cells
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Grid)
	virtual int32 GetIndexFromAddress(FCellAddress Address) const;

	UFUNCTION(BlueprintCallable, Category = Grid)
	virtual bool DoesCellExist(FCellAddress Address) const;

	/*
	*	Override this function in subclasses if you have any navigation logic that cant be
	*	handled with just the include & exclude FGameplayTagContainer filters.
	* 
	*	e.g. You choose to place actors on the grid in the world and can only identify if
	*	its blocking navigation with say an collision overlap check or a GetActorLocation > GetCellAddressFromLocation()
	*	check or something that isn't mapped to a cell address.
	*	(i.e. you do an overlap check at a cells location taking CellSize into account etc)
	* 
	*	Check address before using functions that have bool return types
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Grid)
	virtual bool IsCellBlocked(FCellAddress Address) const;

	UFUNCTION(BlueprintCallable, Category = Grid)
	virtual void SetCellBlocked(FCellAddress Address, bool bBlocked);

	UFUNCTION(BlueprintCallable, Category = Grid)
	virtual FCellInfo GetCellInfoByAddress(FCellAddress Address) const;

	/* Calls GetCellInfoByAddress for each item in array and returns the resulting array */
	UFUNCTION(BlueprintCallable, Category = Grid)
	virtual TArray<FCellInfo> GetCellInfosByAddresses(TArray<FCellAddress> Addresses) const;

	/*
	*	A* (star) Pathfinding
	*	https://medium.com/@nicholas.w.swift/easy-a-star-pathfinding-7e6689c7f7b2
	*/
	UFUNCTION(BlueprintCallable, Category = Grid)
	virtual FAStarSearchResults AStarSearchToGoal(FCellAddress Start, FCellAddress Goal, FGameplayTagContainer InFilters, FGameplayTagContainer ExFilters, bool bCorners);

	UFUNCTION(BlueprintCallable, Category = Grid)
	virtual TArray<FCellInfo> GetCellNeighbors(FCellAddress Address, FGameplayTagContainer InFilters, FGameplayTagContainer ExFilters, bool bCorners = true) const;

	/* You can specify a target array unlike the above that targets TArray<FCellInfo> GridArray */
	//UFUNCTION(BlueprintCallable, Category = Grid)
	//virtual TArray<FCellInfo> GetCellNeighborsExternal(TArray<FCellInfo> CellArray, FCellAddress Address, FGameplayTagContainer InFilters, FGameplayTagContainer ExFilters, bool bCorners = true) const;


	UFUNCTION(BlueprintCallable, Category = Grid)
	virtual TArray<FCellInfo> GetWalkableCells(TArray<FCellInfo> InCellArray) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Grid)
	FIntPoint GetGridExtents() const;

	/*
		Never call this on function one its own as you should never really need to set GridExtents manually.
		Use UpdateGridAsync() or CreateEmptyGrid to create a new grid.
	*/
	void SetGridExtents(FIntPoint Extents);

	UPROPERTY()
	USceneComponent* SceneComp;

	UPROPERTY()
	UBillboardComponent* SpriteComponent;

	//Mesh component for be visual rep for the grid and trace collisions for world locations <> grid address conversions
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grid)
	UStaticMeshComponent* GridMeshComp;

	//Alignment enum not currently used but is meant to choose how to offset grid's 0,0 in relation to actors origin.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Grid)
	EGridMeshAlignment MeshAlignment;

	//the material we'll make and instance of and use for visual debugging
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Grid)
	UMaterial* GridMat;

	UPROPERTY(BlueprintReadWrite, Category = Grid)
	UMaterialInstanceDynamic* GridMatInst;

	//---------
	//Should the actor generate a grid using GridExtents and DefaultCellSize
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	bool bAutoGenerate;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid, meta = (AllowPrivateAccess = "true"))
	FIntPoint GridExtents;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid, meta = (AllowPrivateAccess = "true"))
	FIntPoint DefaultGridExtents = FIntPoint(8, 8);
	//---------

	//All async task should check this to be able to run.
	bool bBusy;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	float CellSize;

	UPROPERTY(BlueprintReadWrite, Category = Grid)
	float DefaultCellSize = 200;

	//Only ever applied to the bAutoGenerate grid on BeginPlay()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	FGameplayTagContainer DefaultCellTags;

	/*
	*	Default place to stick your array of cells. I'm not the boss of you tho so you don't have to use it.
	*	But this actor will use this for preview in editor if you're not generating the grid at runtime.
	*/
	UPROPERTY(BlueprintReadWrite, Category = Grid)
	TArray<FCellInfo> GridArray;

};
