#pragma once

//#include "GameplayTagContainer.h"
#include "Dn2WFCTypes.generated.h"

/* not used */
UENUM(BlueprintType)
enum class ExGridMeshAlignment : uint8
{
	GMOx_xCenter			UMETA(DisplayName = "xCenter"),
	GMOx_xOrigin			UMETA(DisplayName = "xOrigin"),
};

USTRUCT(BlueprintType)
struct FWfcPattern
{
	GENERATED_USTRUCT_BODY()

public:
	/** Default constructor (no initialization). */
	FORCEINLINE FWfcPattern();

	FWfcPattern Rotated90() const
	{
		FWfcPattern result;



		return result;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint PatternSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FColor> PatternArray;
};
FORCEINLINE FWfcPattern::FWfcPattern()
{}


USTRUCT(BlueprintType)
struct FWfcTile
{
	GENERATED_USTRUCT_BODY()

public:
	/** Default constructor (no initialization). */
	FORCEINLINE FWfcTile();

	FName TileName;

	TArray<FName> Top;
	TArray<FName> Left;
	TArray<FName> Right;
	TArray<FName> Bottom;
};
FORCEINLINE FWfcTile::FWfcTile()
{}