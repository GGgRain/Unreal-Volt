//Copyright 2022~2024 DevGrain. All Rights Reserved.


#include "VoltModuleItem.h"

void UVoltModuleItem::ModifySlateVariable(const float DeltaTime,
                                                   const TScriptInterface<IVoltInterface>& Volt)
{
	K2_ModifySlateVariable(DeltaTime, Volt);
}

const TScriptInterface<IVoltInterface>& UVoltModuleItem::GetVoltSlate()
{
	return TargetVoltSlate;
}

void UVoltModuleItem::SetVoltSlate(const TScriptInterface<IVoltInterface>& Slate)
{
	
	TargetVoltSlate = Slate;

}

bool UVoltModuleItem::IsActive()
{
	return K2_IsActive();
}
