//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "Volt_ASA_ChangeBackgroundColor.h"
#include "Volt_ASM_InterpBackgroundColor.h"

UVolt_ASA_ChangeBackgroundColor::UVolt_ASA_ChangeBackgroundColor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UVolt_ASM_InterpBackgroundColor* SlotModule = ObjectInitializer.CreateDefaultSubobject<UVolt_ASM_InterpBackgroundColor>(this,"BackgroundColorInterp");
	Modules.Add(SlotModule);
	SlotModule->RateBasedInterpSpeed = 7;
}

