//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "VoltVariableCollection.h"
#include "VoltVariableBase.h"

UVoltVariableBase* UVoltVariableCollection::FindOrAddVariable(TSubclassOf<UVoltVariableBase> Type)
{
	if(!Type->IsValidLowLevel()) return nullptr;
	
	UVoltVariableBase* FoundVari = FindVariable(Type);

	if(FoundVari) return FoundVari;

	//Add a new one.
	UVoltVariableBase* NewObj = NewObject<UVoltVariableBase>(this,Type);
	
	Variables.Add(NewObj);
	
	return NewObj;
}

UVoltVariableBase* UVoltVariableCollection::FindVariable(TSubclassOf<UVoltVariableBase> Type)
{
	for (UVoltVariableBase* Variable : Variables)
	{
		if(Variable->GetClass() == Type) return Variable;
	}
	
	return nullptr;
}

const TArray<UVoltVariableBase*>& UVoltVariableCollection::GetVariables()
{
	return Variables;
}
