#pragma once

//#include "GameplayTagContainer.h"
#include "GameplayTagContainer.h"
#include "Dn2GameGridTypes.generated.h"

/* not used */
UENUM(BlueprintType)
enum class EGridMeshAlignment : uint8
{
	GMO_Center			UMETA(DisplayName = "Center"),
	GMO_Origin			UMETA(DisplayName = "Origin"),
};


/**
 * A int32 vector in 2D space composed of components (Row, Col) for cell addresses.
 *
 * @return A string describing the vector.
 */
USTRUCT(BlueprintType)
struct FCellAddress
{
	GENERATED_USTRUCT_BODY()

public:
	/** Default constructor (no initialization). */
	FORCEINLINE FCellAddress();
	FORCEINLINE FCellAddress(FIntPoint InAddress);
	FORCEINLINE FCellAddress(int32 InRow, int32 InCol);

	bool operator==(const FCellAddress& OtherAddress) const
	{
		return OtherAddress.Row == Row && OtherAddress.Col == Col;
	}

	bool operator==(const FIntPoint& IntPoint) const
	{
		return IntPoint.X == Row && IntPoint.Y == Col;
	}

	bool operator!=(const FCellAddress& OtherAddress) const
	{
		return OtherAddress.Row != Row || OtherAddress.Col != Col;
	}

	bool operator!=(const FIntPoint& IntPoint) const
	{
		return IntPoint.X != Row || IntPoint.Y != Col;
	}

	friend uint32 GetTypeHash(const FCellAddress& Other)
	{
		return HashCombine(GetTypeHash(Other.Row), GetTypeHash(Other.Col));
	}

	FORCEINLINE FString ToString() const
	{
		return FString::Printf(TEXT("Row:%d Col:%d"), Row, Col);
	}

	/* Row or X coordinate for a grid cell on our grid. 0 indexed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Row;

	/* Column or Y coordinate for a grid cell on our grid. 0 indexed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Col;
};
FORCEINLINE FCellAddress::FCellAddress()
{}
FORCEINLINE FCellAddress::FCellAddress(FIntPoint InAddress)
	: Row(InAddress.X), Col(InAddress.Y)
{}
FORCEINLINE FCellAddress::FCellAddress(int32 InRow, int32 InCol)
	: Row(InRow), Col(InCol)
{}



/**
 *	Used for grid array.
 *	Holds info for each cell in the grid array.
 */
USTRUCT(BlueprintType, meta = (DisplayName = "CellInfo"))
struct FCellInfo
{
	GENERATED_USTRUCT_BODY()

public:
	/** Default constructor (no initialization). */
	FORCEINLINE FCellInfo();
	FORCEINLINE FCellInfo(FCellAddress InAddress);
	FORCEINLINE FCellInfo(FCellAddress InAddress, FGameplayTagContainer InCellTags);
	//FORCEINLINE FCellInfo(FIntPoint InAddress, FVector InLocation, int32 InIndex);

	bool operator==(const FCellInfo& OtherInfo) const
	{
		return OtherInfo.Address == Address;
	}

	void SetBlocked(const bool Blocked)
	{
		bBlocked = Blocked;
	}

	void AddCellTag(FGameplayTag Tag)
	{
		CellTags.AddTag(Tag);
	}

	void AppendCellTags(FGameplayTagContainer Tags)
	{
		CellTags.AppendTags(Tags);
	}

	void RemoveCellTag(FGameplayTag Tag)
	{
		CellTags.RemoveTag(Tag);
	}

	void RemoveCellTags(FGameplayTagContainer Tags)
	{
		CellTags.RemoveTags(Tags);
	}

	void RemoveAllCellTags()
	{
		CellTags.Reset();
	}

	/*
	*	Any negative values in a cell address's Row or Col indicates a value was not assigned.
	* 
	*	Make sure to assign valid, non negative values and check a FCellAddress using DoesCellExist()
	*	before using them.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cell Info")
	FCellAddress Address = FCellAddress(-1, -1);

	/*
		This is now unused, replaced by FGameplayTagContainer include & exclude filters that check against CellTags.

		If all you will ever need is a simple singular navigable check for cells, you can override IsCellBlocked() in
		GridActorBase subclasses and have it return this, leave include & exclude FGameplayTagContainer's empty
		and set bBlocked for cell GridArray's FCellInfo's to modify navigable cells.

		Functions that use IsCellBlocked() are: ABoxGridActor::AStarSearchToGoal(), and ABoxGridActor::GetCellNeighborsFromAddress()

		If you DO use/need IsCellBlocked() to filter for FCellInfo.bBlocked in other functions that return FCellInfo's,
		you can override said function or wrap it in a function and check FCellInfo.bBlocked in IsCellBlocked()'s override.
		--------------

		Simple flag to set or check if a cell is blocked.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cell Info")
	bool bBlocked;

	/* Tags we can use for filtering of cells, for more robust blocking and navigation checks, etc, than just using bBlocked */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cell Info")
	FGameplayTagContainer CellTags;
};
FORCEINLINE FCellInfo::FCellInfo()
{}
FORCEINLINE FCellInfo::FCellInfo(FCellAddress InAddress)
	: Address(InAddress)
{}
FORCEINLINE FCellInfo::FCellInfo(FCellAddress InAddress, FGameplayTagContainer InCellTags)
	: Address(InAddress), CellTags(InCellTags)
{}
/*FORCEINLINE FCellInfo::FCellInfo(FIntPoint InAddress, FVector InLocation, int32 InIndex)
	: Address(InAddress), Location(InLocation), Index(InIndex)
{}*/



/**
 *	Used for the A* algo
 */
USTRUCT(BlueprintType, meta = (DisplayName = "AStar Cell Info"))
struct FAStarCellInfo
{
	GENERATED_USTRUCT_BODY()

public:
	/** Default constructor (no initialization). */
	FORCEINLINE FAStarCellInfo();
	FORCEINLINE FAStarCellInfo(FCellAddress InAddress, FCellAddress InCameFrom);
	FORCEINLINE FAStarCellInfo(float InH, float InG, FCellAddress InCameFrom, FCellAddress InAddress);


	bool operator==(const FAStarCellInfo& OtherItem) const
	{
		return OtherItem.Address == Address;
	}

	bool operator==(const FIntPoint& IntPoint) const
	{
		return IntPoint.X == Address.Row && IntPoint.Y == Address.Col;
	}

	friend uint32 GetTypeHash(const FAStarCellInfo& Other)
	{
		return HashCombine(GetTypeHash(Other.Address.Row), GetTypeHash(Other.Address.Col));
	}

	//Heuristic, estimated distance from this cell to target
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Cell Info")
	float H;

	//Distance Traveled from start cell to this cell
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Cell Info")
	float G;

	//Total cost
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Cell Info")
	float F;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Cell Info")
	FCellAddress Address;

	//direction we came from
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Cell Info")
	FCellAddress CameFrom;
};
FORCEINLINE FAStarCellInfo::FAStarCellInfo()
{}
FORCEINLINE FAStarCellInfo::FAStarCellInfo(FCellAddress InAddress, FCellAddress InCameFrom)
	: Address(InAddress), CameFrom(InCameFrom)
{}
FORCEINLINE FAStarCellInfo::FAStarCellInfo(float InH, float InG, FCellAddress InCameFrom, FCellAddress InAddress)
	: H(InH), G(InG), Address(InAddress), CameFrom(InCameFrom)
{}



/**
 *	Struct holding the results of AStar search algorithm.
 */
USTRUCT(BlueprintType, meta = (DisplayName = "AStar Search Info"))
struct FAStarSearchResults
{
	GENERATED_USTRUCT_BODY()

public:

	/** Default constructor (no initialization). */
	FORCEINLINE FAStarSearchResults();
	FORCEINLINE FAStarSearchResults(TMap<FCellAddress, FCellAddress> InCameFrom, TMap<FCellAddress, int32> InCostSoFar);

	/*
	*	True if we found an "unblocked" path to our goal cell. If false, we did not find a path to our goal.
	*	The path (an array of FCellAddress), will be return up to the point of failure or the full path if successful.
	* 
	* 
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Search Results")
	bool bFoundGoal;

	/* this might be unused. idk */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Search Results")
	TMap<FCellAddress, FCellAddress> CameFrom;

	/* this might be unused. idk */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Search Results")
	TMap<FCellAddress, int32> CostSoFar;

	/* The resulting complete or incomplete path to our goal in the form of an array of FCellAddress. Start cell not included */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Search Results")
	TArray<FCellAddress> Path;
};

FORCEINLINE FAStarSearchResults::FAStarSearchResults()
{}

FORCEINLINE FAStarSearchResults::FAStarSearchResults(TMap<FCellAddress, FCellAddress> InCameFrom, TMap<FCellAddress, int32> InCostSoFar)
	: CameFrom(InCameFrom), CostSoFar(InCostSoFar)
{}