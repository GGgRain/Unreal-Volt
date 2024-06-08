// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "VoltVariableBase.h"
#include "VoltVariableActionBase.h"
#include "Templates/SubclassOf.h"

#include "VoltSettings.generated.h"


/**
 * An array wrapper for the UVoltVariableActionBase sub classes.
 */

USTRUCT(BlueprintType)
struct FVoltVariableActionList
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category="Variable Actions")
	TArray<TSubclassOf<UVoltVariableActionBase>> Actions;
	
};

/**
 * The Developer Settings class that holds the fundamental data for Volt.
 */

UCLASS(config = VoltSetting, Defaultconfig, meta = (DisplayName = "Volt Settings"))
class VOLTCORE_API UVoltSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UVoltSettings();

public:
	
	/**
	 * The actions this variable will execute when this variable need to be applied.
	 * Variable will execute the first action that fits with the provided slate.
	 * Modify it to make the variables support any additional slate types that are not originated from the unreal slate architecture.
	 */
	UPROPERTY(config, EditAnywhere, Category="Volt Variable")
	TMap<TSubclassOf<UVoltVariableBase>,FVoltVariableActionList> VariableActionsForVariable;
};
