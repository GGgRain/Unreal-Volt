//Copyright 2022~2024 DevGrain. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VoltInterface.generated.h"

class SWidget;
class UVoltVariableCollection;

/**
 * Basic interface for the animated slate. it can be used both in non UCLASS, USTRUCT based classes and UObject based classes.
 * Check out the UVoltProxy and Volt native slate types.
 */

UINTERFACE(Blueprintable)
class VOLTCORE_API UVoltInterface : public UInterface
{
	GENERATED_BODY()
};


class VOLTCORE_API IVoltInterface
{
	GENERATED_BODY()

public:

	IVoltInterface() {}

public:

	//Get the variable collection. It will trigger the native GetVoltVariableCollection() internally.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Slate Interface Action", DisplayName="Get Animated Slate Variable Collection")
	UVoltVariableCollection* K2_GetVoltVariableCollection();

	//Get the variable collection. It will trigger the native GetVoltVariableCollection() internally.
	virtual UVoltVariableCollection* K2_GetVoltVariableCollection_Implementation() { return GetVoltVariableCollection(); }

	//Get the variable collection.
	virtual UVoltVariableCollection* GetVoltVariableCollection() { return nullptr; }

public:
	
	//Get the slate reference.
	virtual TWeakPtr<SWidget> GetTargetSlate() { return nullptr; }
};