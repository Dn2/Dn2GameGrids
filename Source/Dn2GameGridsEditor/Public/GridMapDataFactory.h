// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "GridMapDataFactory.generated.h"

/**
 * 
 */
UCLASS()
class DN2GAMEGRIDSEDITOR_API UGridMapDataFactory : public UFactory
{
	GENERATED_BODY()
	UGridMapDataFactory(const FObjectInitializer& ObjectInitializer);
	
public:
	virtual UObject* FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled) override;
};
