// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Async/AsyncWork.h"
//#include "Dn2GameGridTypes.h"
//#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
#include "GridActorBase.h"
#include "BoxGridActor.generated.h"

UCLASS()
class DN2GAMEGRIDS_API ABoxGridActor : public AGridActorBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoxGridActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//virtual void OnConstruction(const FTransform& Transform) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;
#endif

	virtual bool UpdateGridAsync(FIntPoint Extents, float GridCellSize, FGameplayTagContainer DefaultTags) override;
	virtual bool GetPathToGoalAsync(FCellAddress Start, FCellAddress Goal, FGameplayTagContainer InFilter, FGameplayTagContainer ExFilter, bool bConers) override;


	//UFUNCTION(BlueprintCallable, Category = Grid)
	virtual TArray<FCellInfo> CreateEmptyGrid(int32 XExtent, int32 YExtent, FGameplayTagContainer DefaultTags) override;

	//UFUNCTION(BlueprintCallable, Category = Grid)
	virtual FVector GetCellLocationFromAddress(FCellAddress Address) override;

	virtual FCellAddress GetCellAddressFromLocation(FVector Location) override;

	virtual int32 GetIndexFromAddress(FCellAddress Address) const override;

	//Cheap but assumes cells index in array. check address before using
	virtual bool DoesCellExist(FCellAddress Address) const override;

	//Cheap but assumes cells index in array. check address before using
	//virtual bool IsCellBlocked(FCellAddress Address) const override;

	//virtual void SetCellBlocked(FCellAddress Address, bool bBlocked);

	virtual FCellInfo GetCellInfoByAddress(FCellAddress Address) const override;

	virtual FAStarSearchResults AStarSearchToGoal(FCellAddress Start, FCellAddress Goal, FGameplayTagContainer InFilters, FGameplayTagContainer ExFilters, bool bCorners) override;

	virtual TArray<FCellInfo> GetCellNeighbors(FCellAddress Address, FGameplayTagContainer InFilters, FGameplayTagContainer ExFilters, bool bCorners = true) const;



	UFUNCTION(/*BlueprintNativeEvent, Category = Grid*/)
	virtual void OnUpdateGrid_Internal(const FIntPoint& OutGridExtents, const TArray<FCellInfo>& OutCellArray, const float OutCellSize);

	UFUNCTION(BlueprintImplementableEvent, Category = Grid/**/)
	void OnUpdateGrid(const FIntPoint& OutGridExtents, const TArray<FCellInfo>& OutCellArray, const float OutCellSize);

	UFUNCTION()
	virtual void OnAStarSearchEnd_Internal(const FAStarSearchResults& AStarSearchResults, const bool GoalFound, const int32 NumberOfCells, const FCellAddress& LastCellFound);

	UFUNCTION(BlueprintImplementableEvent, Category = Grid)
	void OnAStarSearchEnd(const FAStarSearchResults& AStarSearchResults, const bool GoalFound, const int32 NumberOfCells, const FCellAddress& LastCellFound);

};

/*
*
*/
class FUpdateGridTask : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<FUpdateGridTask>;

public:
	FUpdateGridTask(ABoxGridActor* gridActor, FIntPoint gridSize, float cellSize=100) :
		GridActor(gridActor),
		GridSize(gridSize),
		GridCellSize(cellSize)
	{}

protected:
	ABoxGridActor* GridActor;
	FIntPoint GridSize;
	float GridCellSize;

	void DoWork()
	{
		if (GridActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Running from async..."));

			//create a grid array of cells
			TArray<FCellInfo> NewCellArray = GridActor->CreateEmptyGrid(GridSize.X, GridSize.Y, FGameplayTagContainer::EmptyContainer);

			//Store assign it to our actor
			GridActor->GridArray = NewCellArray;
			GridActor->SetGridExtents(GridSize);
			GridActor->CellSize = GridCellSize;

			//Broadcast our event so we can do post grid creation stuff
			GridActor->OnUpdateGridDelCPP.Broadcast(GridSize, GridActor->GridArray, GridCellSize);
		}
	}
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FUpdateGridTask, STATGROUP_ThreadPoolAsyncTasks);
	}
};


/*
*
*/
class FStarSearchToGoalTask : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<FStarSearchToGoalTask>;

public:
	FStarSearchToGoalTask(ABoxGridActor* gridActor, FCellAddress start, FCellAddress goal, FGameplayTagContainer inFilter, FGameplayTagContainer exFilter, bool bcorners = true) :
		GridActor(gridActor),
		Start(start),
		Goal(goal),
		InFilter(inFilter),
		ExFilter(exFilter),
		bCorners(bcorners)
	{}

protected:
	ABoxGridActor* GridActor;
	FCellAddress Start;
	FCellAddress Goal;
	FGameplayTagContainer InFilter;
	FGameplayTagContainer ExFilter;
	bool bCorners;

	void DoWork()
	{
		FAStarSearchResults Results = GridActor->AStarSearchToGoal(Start, Goal, InFilter, ExFilter, bCorners);

		if (Results.Path.IsValidIndex(0))
		{
			GridActor->OnAStarSearchEndDelCPP.Broadcast(Results, Results.bFoundGoal, Results.Path.Num(), Results.Path.Last());
		}
		else
		{
			GridActor->OnAStarSearchEndDelCPP.Broadcast(Results, Results.bFoundGoal, 0, FCellAddress());
		}

	}
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FStarSearchToGoalTask, STATGROUP_ThreadPoolAsyncTasks);
	}
};