// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "GridMapDataFactoryNew.generated.h"

/**
 * 
 */
UCLASS()
class DN2GAMEGRIDSEDITOR_API UGridMapDataFactoryNew : public UFactory
{
	GENERATED_BODY()
	UGridMapDataFactoryNew(const FObjectInitializer& ObjectInitializer);
	
public:
	//~ UFactory Interface
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool ShouldShowInNewMenu() const override;
	
	/** Returns the name of the factory for menus */
	virtual FText GetDisplayName() const override;
	
	virtual FText GetToolTip() const override;
};
