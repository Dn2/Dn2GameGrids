// Fill out your copyright notice in the Description page of Project Settings.


#include "GridActorBase.h"

// Sets default values
AGridActorBase::AGridActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = SceneComp;

	SpriteComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("IconSpriteComponent"));
	SpriteComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	GridMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridStaticMeshComponent"));
	GridMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//RootComponent = GridMeshComp;

	if (GridMeshComp/* && GridMesh*/)
	{
		//GridMeshComp->SetStaticMesh(GridMesh);
	}


	//set default CellSize if none is set
	if (CellSize < 10)
	{
		CellSize = DefaultCellSize;
	}

	if (GridExtents.X < 2)
	{
		GridExtents.X = DefaultGridExtents.X;
	}

	if (GridExtents.Y < 2)
	{
		GridExtents.Y = DefaultGridExtents.Y;
	}
}

// Called when the game starts or when spawned
void AGridActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGridActorBase::SetBusy(bool bIsBusy)
{
	bBusy = bIsBusy;
}

bool AGridActorBase::IsBusy()
{
	return bBusy;
}

// Called every frame
void AGridActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGridActorBase::UpdateGridAsync(FIntPoint Extents, float GridCellSize, FGameplayTagContainer DefaultTags)
{
	return false;
}

bool AGridActorBase::GetPathToGoalAsync(FCellAddress Start, FCellAddress Goal, FGameplayTagContainer InFilter, FGameplayTagContainer ExFilter, bool bConers)
{
	return false;
}

TArray<FCellInfo> AGridActorBase::CreateEmptyGrid(int32 XExtent, int32 YExtent, FGameplayTagContainer DefaultTags)
{
	return TArray<FCellInfo>();
}

FVector AGridActorBase::GetCellLocationFromAddress(FCellAddress Address)
{
	return FVector();
}

FCellAddress AGridActorBase::GetCellAddressFromLocation(FVector Location)
{
	return FCellAddress(-1, -1);
}

int32 AGridActorBase::GetIndexFromAddress(FCellAddress Address) const
{
	return -1;
}

bool AGridActorBase::DoesCellExist(FCellAddress) const
{
	return false;
}

bool AGridActorBase::IsCellBlocked(FCellAddress Address) const
{
	return false;
}

void AGridActorBase::SetCellBlocked(FCellAddress Address, bool bBlocked)
{
	if (DoesCellExist(Address))
	{
		int32 Index = (GetGridExtents().Y * Address.Row) + Address.Col;
		GridArray[Index].SetBlocked(bBlocked);
	}
}


FCellInfo AGridActorBase::GetCellInfoByAddress(FCellAddress Address) const
{
	return FCellInfo();
}

TArray<FCellInfo> AGridActorBase::GetCellInfosByAddresses(TArray<FCellAddress> Addresses) const
{
	TArray<FCellInfo> CellResults;

	for (FCellAddress AddressItem : Addresses)
	{
		if (DoesCellExist(AddressItem))
		{
			CellResults.Add(GetCellInfoByAddress(AddressItem));
		}
	}

	return CellResults;
}

FAStarSearchResults AGridActorBase::AStarSearchToGoal(FCellAddress Start, FCellAddress Goal, FGameplayTagContainer InFilters, FGameplayTagContainer ExFilters, bool bCorners)
{
	return FAStarSearchResults();
}

TArray<FCellInfo> AGridActorBase::GetCellNeighbors(FCellAddress Address, FGameplayTagContainer InFilters, FGameplayTagContainer ExFilters, bool bCorners /*= true*/) const
{
	return TArray<FCellInfo>();
}

TArray<FCellInfo> AGridActorBase::GetWalkableCells(TArray<FCellInfo> InCellArray) const
{
	return TArray<FCellInfo>();
}

FIntPoint AGridActorBase::GetGridExtents() const
{
	return GridExtents;
}

void AGridActorBase::SetGridExtents(FIntPoint Extents)
{
	if (Extents.X < 2)
	{
		Extents.X = DefaultGridExtents.X;
	}

	if (Extents.Y < 2)
	{
		Extents.Y = DefaultGridExtents.Y;
	}

	GridExtents = Extents;
}


