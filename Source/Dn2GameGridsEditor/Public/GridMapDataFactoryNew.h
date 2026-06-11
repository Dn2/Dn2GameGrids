// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include  "JsonUtilities/Public/JsonUtilities.h"
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
	
	//virtual UObject* FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled) override;
	
	/** Returns the name of the factory for menus */
	virtual FText GetDisplayName() const override;
	
	virtual FText GetToolTip() const override;
	
protected:
	//virtual UObject* FactoryCreateText(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn) override;
};
