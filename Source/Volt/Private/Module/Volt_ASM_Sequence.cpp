//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "Volt_ASM_Sequence.h"
#include "VoltInterface.h"

void UVolt_ASM_Sequence::PickupNextModule()
{
	CurrentlyPlayingModuleIdx++;

	if(bShouldLoop)
	{
		CurrentlyPlayingModuleIdx = CurrentlyPlayingModuleIdx % Modules.Num();
	}
}

void UVolt_ASM_Sequence::ModifySlateVariable(const float DeltaTime,
                                             const TScriptInterface<IVoltInterface>& Volt)
{
	if(Modules.IsValidIndex(CurrentlyPlayingModuleIdx))
	{
		Modules[CurrentlyPlayingModuleIdx]->SetVoltSlate(Volt);
		
		if(Modules[CurrentlyPlayingModuleIdx]->IsActive())
		{
			Modules[CurrentlyPlayingModuleIdx]->ModifySlateVariable(DeltaTime,Volt);
		}else
		{
			PickupNextModule();
		}
	}
	
	bEverUpdated = true;
}

bool UVolt_ASM_Sequence::IsActive()
{
	if(!bEverUpdated) return true;
	
	return 	Modules.Num() > CurrentlyPlayingModuleIdx;
}
