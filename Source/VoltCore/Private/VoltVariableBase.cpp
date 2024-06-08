//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "VoltVariableBase.h"

#include "VoltSettings.h"
#include "VoltVariableActionBase.h"

bool UVoltVariableBase::CheckCachedActions()
{
	return bCachedActions;
}

void UVoltVariableBase::CacheActions()
{
	//Clear caches first.
	ClearCachedActions();

	//Mark that we already cached the data even if it might fail at the cache action.
	bCachedActions = true;
	
	//check some possible fails.
	const UVoltSettings* DevSetting = GetDefault<UVoltSettings>();

	if (!DevSetting) return;

	if(!DevSetting->VariableActionsForVariable.Contains(this->GetClass())) return;

	const TArray<TSubclassOf<UVoltVariableActionBase>>& VariableActionsForThisVariable = DevSetting->VariableActionsForVariable[this->GetClass()].Actions;
	
	//Make a new action bases for the variable.
	CachedActions.Reserve(VariableActionsForThisVariable.Num());
	
	for (TSubclassOf<UVoltVariableActionBase> Action : VariableActionsForThisVariable)
	{
		UVoltVariableActionBase* NewAction = NewObject<UVoltVariableActionBase>(this,Action);
		CachedActions.Emplace(NewAction);
	}
}

void UVoltVariableBase::ClearCachedActions()
{
	CachedActions.Empty();
	bCachedActions = false;
}

void UVoltVariableBase::ApplyVariable(const TWeakPtr<SWidget>& SlateToApply)
{
	if(!CheckCachedActions()) CacheActions();
	
	for (UVoltVariableActionBase* ActionBase : CachedActions)
	{
		if (!ActionBase) continue;

		if (ActionBase->CheckSupportWidget(SlateToApply))
		{
			ActionBase->ApplyVariable(this, SlateToApply);

			break;
		}
	}
}
