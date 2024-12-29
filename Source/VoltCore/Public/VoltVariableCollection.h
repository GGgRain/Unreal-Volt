//Copyright 2022~2024 DevGrain. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VoltVariableBase.h"
#include "Templates/SubclassOf.h"
#include "VoltVariableCollection.generated.h"


/*
 * An object that holds all the variables for the animated slate.
 */

UCLASS(BlueprintType)
class VOLTCORE_API UVoltVariableCollection : public UObject
{
	GENERATED_BODY()

public:
	
	virtual ~UVoltVariableCollection() = default;

	UVoltVariableCollection() = default;

public:

	UFUNCTION(BlueprintCallable, Category="Animated Slate Variable")
	UVoltVariableBase* FindOrAddVariable(TSubclassOf<UVoltVariableBase> Type);
	
	UFUNCTION(BlueprintCallable, Category="Animated Slate Variable")
	UVoltVariableBase* FindVariable(TSubclassOf<UVoltVariableBase> Type);

public:

	UFUNCTION(BlueprintCallable, Category="Animated Slate Variable")
	const TArray<UVoltVariableBase*>& GetVariables();


private:

	UVoltVariableBase* EnqueueVariableOnQueue(UVoltVariableBase* Variable);

	void ProcessQueue();

	friend UVoltAnimationManager;

private:

	UPROPERTY(Transient)
	TArray<UVoltVariableBase*> QueuedVariables;
	
private:

	UPROPERTY(Transient)
	TArray<UVoltVariableBase*> Variables;
};