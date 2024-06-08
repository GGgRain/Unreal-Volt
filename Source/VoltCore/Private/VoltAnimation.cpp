//Copyright 2022~2024 DevGrain. All Rights Reserved.


#include "VoltAnimation.h"
#include "VoltModuleItem.h"


UVoltModuleItem* UVoltAnimation::K2_GetModuleForClass(const TSubclassOf<UVoltModuleItem> TargetModuleClass)
{
	if (Modules.IsEmpty()) return nullptr; // Inactive since there is no module to play.

	for (UVoltModuleItem* Module : Modules)
	{
		if (Module->GetClass() == TargetModuleClass)
		{
			return Module;
		}
	}

	return nullptr;
}

const bool UVoltAnimation::IsActive() const
{
	if (Modules.IsEmpty()) return false; // Inactive since there is no module to play.

	for (UVoltModuleItem* Module : Modules)
	{
		if (!Module) continue; //Skip the check if not valid.

		if (Module->IsActive()) return true;
	}

	return false;
}
