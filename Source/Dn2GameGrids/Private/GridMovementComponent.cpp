// Fill out your copyright notice in the Description page of Project Settings.


#include "GridMovementComponent.h"

// Sets default values for this component's properties
UGridMovementComponent::UGridMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	MoveLerpTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MoveLerpTLComp"));

	if (GameSpeed <= 0.0f)
		GameSpeed = 1;
}


// Called when the game starts
void UGridMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InterpMoveFunction.BindUFunction(this, FName{ TEXT("OnTimelineTick") });
	InterpFinishMoveFunction.BindUFunction(this, FName{ TEXT("OnTimelineEnd") });

	//Create curve based on GameSpeed for our TimeLine
	fCurve = NewObject<UCurveFloat>();
	fCurve->bIsEventCurve = true;
	fCurve->FloatCurve.AddKey(0.0f, 0.0f);
	fCurve->FloatCurve.AddKey(GameSpeed, 1.0f);

	if (MoveLerpTimeline && fCurve)
	{
		MoveLerpTimeline->AddInterpFloat(fCurve, InterpMoveFunction, FName{ TEXT("Floaty") });
		MoveLerpTimeline->SetTimelineFinishedFunc(InterpFinishMoveFunction);

		//UE_LOG(LogTemp, Warning, TEXT("TimeLine and Curve not null! new"));
	}
}


// Called every frame
void UGridMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGridMovementComponent::OnTimelineTick(float val)
{
	AActor* OwningActor = GetOwner();
	if (OwningActor && !OwningActor->IsPendingKillPending() && TargetPath.IsValidIndex(0))
	{
		if (val == 0.0f)
		{
			FromAddress = AddressOnGrid;
			UpdateGridLocationData(TargetPath[0]);
			ToAddress = AddressOnGrid;

			//++CurrentTurn;
			//OnTimelineBegin();
			OnTimelineStartDelegate.Broadcast();
		}

		//UAStarGISubsystem* MySubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UAStarGISubsystem>();

		if (OwningGrid)
		{
			FVector TargetLoc = OwningGrid->GetCellLocationFromAddress(ToAddress);
			OwningActor->SetActorLocation(FMath::LerpStable(OwningGrid->GetCellLocationFromAddress(FromAddress), TargetLoc, val));
		}
		//UE_LOG(LogTemp, Warning, TEXT("Ran: GM Tick = %f"), val);

	}

	OnTimelineTickDelegate.Broadcast(val);
}

void UGridMovementComponent::OnTimelineLocationChanged(bool Placed, bool SilentMove)
{

}

void UGridMovementComponent::OnTimelineBegin()
{

}

void UGridMovementComponent::OnTimelineEnd()
{
	AActor* OwningActor = GetOwner();
	//stop if bPendingMovementInterrupt is true and return
	if (bPendingMovementInterrupt)
	{
		//PlayerActor->LastAddress = PlayerActor->Address;
		bPendingMovementInterrupt = false;

		//remove previous address from TargetPath
		if (OwningActor && TargetPath.Num() > 0)
		{
			TargetPath.RemoveAt(0);
			//PlayerActor->LastAddress = PlayerActor->Address;
		}

		OnTimelineEndDelegate.Broadcast();

		return;
	}

	//Continue playing another movement loop if there is no pending interrupts TargetPath isn't empty
	if (OwningActor && TargetPath.Num() > 0)
	{
		TargetPath.RemoveAt(0);

		if (!bPendingMovementInterrupt && TargetPath.Num() > 0)
		{
			//PlayerActor->LastAddress = PlayerActor->Address;
			MoveLerpTimeline->PlayFromStart();
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("TimeLine Finished Function Called!"));

	OnTimelineEndDelegate.Broadcast();
}


bool UGridMovementComponent::IsMoving()
{
	return MoveLerpTimeline && MoveLerpTimeline->IsPlaying();
}


void UGridMovementComponent::StartMove(bool bUnpaused)
{
	if (MoveLerpTimeline && fCurve && !MoveLerpTimeline->IsPlaying() && TargetPath.IsValidIndex(0))
	{
		if (bUnpaused)
		{
			/* 
			*	normally set by the first tick of the timeline. since this isnt from the tick if its unpausing, we do it here
			*/
			FromAddress = AddressOnGrid;
			UpdateGridLocationData(TargetPath[0]);
			ToAddress = AddressOnGrid;

			// should add param for passing if was unpaused
			OnTimelineStartDelegate.Broadcast();

			MoveLerpTimeline->Play();
		}
		else
		{
			MoveLerpTimeline->PlayFromStart();
		}

	}
}


bool UGridMovementComponent::SetMovePath(TArray<FCellAddress> InPath, bool bAutoPlay)
{
	if (InPath.IsValidIndex(0))
	{
		TargetPath = InPath;


		if (bAutoPlay && MoveLerpTimeline && fCurve)
		{
			StartMove(false);
		}
	}
	
	return false;
}

bool UGridMovementComponent::PauseMove(bool bFinishCurrent)
{
	if (MoveLerpTimeline && MoveLerpTimeline->IsPlaying())
	{
		if (bFinishCurrent)
		{
			bPendingMovementInterrupt = true;
		}
		else
		{
			MoveLerpTimeline->Stop();
			return true;
		}
	}

	return false;
}

bool UGridMovementComponent::SetActorGridLocation(AGridActorBase* GridActor, FCellAddress InAddress, bool Placed/*=true*/, bool SilentMove/*=true*/)
{
	AActor* OwningActor = GetOwner();

/*
	UE_LOG(LogTemp, Warning, TEXT("OwningActor IsValid: %s"), (GridActor->DoesCellExist(InAddress) ? TEXT("true") : TEXT("false")));
	UE_LOG(LogTemp, Warning, TEXT("SetLocGridActor InHere: %s"), *InAddress.ToString());*/

	if (GridActor && GridActor->DoesCellExist(InAddress) && OwningActor && !OwningActor->IsPendingKillPending())
	{
		OwningGrid = GridActor;
		OwningActor->SetActorLocation(OwningGrid->GetCellLocationFromAddress(InAddress), false);
		UpdateGridLocationData(InAddress);

		OnTimelineLocationChangedDelegate.Broadcast(Placed, SilentMove);
		return true;
	}

	return false;
}

void UGridMovementComponent::UpdateGridLocationData(FCellAddress NewAddress)
{
	AddressOnGrid = NewAddress;
}

