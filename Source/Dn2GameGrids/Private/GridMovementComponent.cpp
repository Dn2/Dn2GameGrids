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

void UGridMovementComponent::StartMove()
{
	if (MoveLerpTimeline && fCurve && !MoveLerpTimeline->IsPlaying())
	{
		MoveLerpTimeline->PlayFromStart();
	}
}


bool UGridMovementComponent::SetActorGridLocation(AGridActorBase* GridActor, FCellAddress InAddress, bool Placed/*=true*/, bool SilentMove/*=true*/)
{
	AActor* OwningActor = GetOwner();
	if (OwningGrid && OwningGrid->DoesCellExist(InAddress) && OwningActor && !OwningActor->IsPendingKillPending())
	{
		OwningActor->SetActorLocation(OwningGrid->GetCellLocationFromAddress(InAddress), false);
		UpdateGridLocationData(InAddress);

		OnTimelineLocationChangedDelegate.Broadcast(Placed, SilentMove);
	}

	return false;
}

void UGridMovementComponent::UpdateGridLocationData(FCellAddress NewAddress)
{
	AddressOnGrid = NewAddress;
}

