//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "VoltVariableCollection.h"
#include "VoltVariableBase.h"

UVoltVariableBase* UVoltVariableCollection::FindOrAddVariable(TSubclassOf<UVoltVariableBase> Type)
{
	if(!Type->IsValidLowLevel()) return nullptr;

	if(UVoltVariableBase* FoundVariable = FindVariable(Type)) return FoundVariable;

	//Add a new one.
	return EnqueueVariableOnQueue(NewObject<UVoltVariableBase>(this,Type));;
}

UVoltVariableBase* UVoltVariableCollection::FindVariable(TSubclassOf<UVoltVariableBase> Type)
{

	UVoltVariableBase** Item = nullptr;

	Item = QueuedVariables.FindByPredicate([Type] (const UVoltVariableBase* CheckVariable = nullptr) {
		if(CheckVariable == nullptr) return false;
		return  CheckVariable->GetClass() == Type;
	});
	
	if(Item != nullptr) return *Item;

	Item = Variables.FindByPredicate([Type] (const UVoltVariableBase* CheckVariable = nullptr) {
		if(CheckVariable == nullptr) return false;
		return  CheckVariable->GetClass() == Type;
	});
	
	return (Item != nullptr) ? *Item : nullptr;
}

const TArray<UVoltVariableBase*>& UVoltVariableCollection::GetVariables()
{
	return Variables;
}

UVoltVariableBase* UVoltVariableCollection::EnqueueVariableOnQueue(UVoltVariableBase* Variable)
{
	if(!Variable || !Variable->IsValidLowLevel()) return nullptr;
	
	if(!QueuedVariables.ContainsByPredicate([Variable] (const UVoltVariableBase* CheckVariable) {
		return CheckVariable->GetClass() == Variable->GetClass();
	})
	&&
	!Variables.ContainsByPredicate([Variable] (const UVoltVariableBase* CheckVariable) {
		return CheckVariable->GetClass() == Variable->GetClass();
	}))
	{
		QueuedVariables.Add(Variable);
		return Variable;
	}

	return nullptr;
}

void UVoltVariableCollection::ProcessQueue()
{
	for (UVoltVariableBase* QueuedVariable : QueuedVariables)
	{
		if(QueuedVariable == nullptr) continue;
		Variables.Add(QueuedVariable);
	}

	QueuedVariables.Empty();
}

