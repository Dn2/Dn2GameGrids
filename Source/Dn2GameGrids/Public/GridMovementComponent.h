// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "Dn2GameGridTypes.h"
#include "GridActorBase.h"
#include "GridMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimelineTickDel, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimelineLocationChangedDel, float, Placed, float, SilentMove);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimelineStartDel, bool, bFromPaused);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimelineEndDel);

UCLASS( ClassGroup=(Grid), meta=(BlueprintSpawnableComponent) )
class DN2GAMEGRIDS_API UGridMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGridMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	UTimelineComponent* MoveLerpTimeline;

	UPROPERTY()
	UCurveFloat* fCurve;

	UPROPERTY()
	FOnTimelineFloat InterpMoveFunction {};

	UPROPERTY()
	FOnTimelineEvent InterpFinishMoveFunction {};



	UFUNCTION(BlueprintCallable, Category = Grid)
	void OnTimelineTick(float val);

	/*
	*	@Placed: was our actor-owner placed here or was it part of a bigger set of movement
	*	@SilentMove: if true the move is intended to alternately not have normal move effects
	*
	*/
	UFUNCTION(BlueprintCallable, Category = Grid)
	void OnTimelineLocationChanged(bool Placed, bool SilentMove);

	UFUNCTION(BlueprintCallable, Category = Grid)
	void OnTimelineBegin();
	
	UFUNCTION(BlueprintCallable, Category = Grid)
	void OnTimelineEnd();

	UFUNCTION(BlueprintCallable, Category = Grid)
	bool IsMoving();

	/* bUnpause = play from current position, i.e. from the beginning of timeline or center if grid */
	UFUNCTION(BlueprintCallable, Category = Grid)
	bool StartMove(bool bUnpaused);

	UFUNCTION(BlueprintCallable, Category = Grid)
	bool SetMovePath(TArray<FCellAddress> InPath, bool bAutoPlay);


	//Stop after current "cell to cell" lerp but keep leftover path.
	bool PauseMove(bool bFinishCurrent);


	bool StopMove(bool bFinishCurrent, bool bStayAtTarget = true, bool bClearPath = false);


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Grid)
	float GameSpeed;

	UPROPERTY(BlueprintReadWrite, Category = Grid)
	float CurrentTurn;

	//Lets the component know that we need to stop after the last move to a point is complete.
	bool bPendingMovementInterrupt;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Grid")
	FOnTimelineTickDel OnTimelineTickDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Grid")
	FOnTimelineLocationChangedDel OnTimelineLocationChangedDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Grid")
	FOnTimelineStartDel OnTimelineStartDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Grid")
	FOnTimelineEndDel OnTimelineEndDelegate;


	UPROPERTY(BlueprintReadOnly, Category = Grid)
	AGridActorBase* OwningGrid;

	UPROPERTY(BlueprintReadWrite, Category = Grid)
	FCellAddress AddressOnGrid;

	FCellAddress FromAddress;
	FCellAddress ToAddress;

	UPROPERTY(BlueprintReadWrite, Category = Grid)
	TArray<FCellAddress> TargetPath;

	UFUNCTION(BlueprintCallable, Category = Grid)
	virtual bool SetActorGridLocation(AGridActorBase* GridActor, FCellAddress InAddress, bool Placed=true, bool SilentMove=true);

	UFUNCTION(BlueprintCallable, Category = Grid)
	virtual void SetGridLocationData(FCellAddress NewAddress);

	UFUNCTION(BlueprintCallable, BlueprintPure,Category = Grid)
	virtual FCellAddress GetGridLocationData();
};
