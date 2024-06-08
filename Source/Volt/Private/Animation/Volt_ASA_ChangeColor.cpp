//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "Volt_ASA_ChangeColor.h"
#include "Volt_ASM_InterpColor.h"

UVolt_ASA_ChangeColor::UVolt_ASA_ChangeColor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UVolt_ASM_InterpColor* SlotModule = ObjectInitializer.CreateDefaultSubobject<UVolt_ASM_InterpColor>(this,"ColorInterp");
	Modules.Add(SlotModule);
	SlotModule->InterpSpeed = 7;
}

