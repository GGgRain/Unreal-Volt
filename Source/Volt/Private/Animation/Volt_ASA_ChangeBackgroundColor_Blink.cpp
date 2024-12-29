//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "Volt_ASA_ChangeBackgroundColor_Blink.h"
#include "Volt_ASM_InterpBackgroundColor.h"
#include "Volt_ASM_Sequence.h"


UVolt_ASA_ChangeBackgroundColor_Blink::UVolt_ASA_ChangeBackgroundColor_Blink(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UVolt_ASM_Sequence* SequenceModule = ObjectInitializer.CreateDefaultSubobject<UVolt_ASM_Sequence>(this,"Sequence");
	Modules.Add(SequenceModule);

	UVolt_ASM_InterpBackgroundColor* ColorInModule = ObjectInitializer.CreateDefaultSubobject<UVolt_ASM_InterpBackgroundColor>(this,"ColorInterpIn");
	ColorInModule->RateBasedInterpSpeed = 10;

	UVolt_ASM_InterpBackgroundColor* ColorOutModule = ObjectInitializer.CreateDefaultSubobject<UVolt_ASM_InterpBackgroundColor>(this,"ColorInterpOut");
	ColorOutModule->RateBasedInterpSpeed = 10;

	SequenceModule->Modules.Add(ColorInModule);
	SequenceModule->Modules.Add(ColorOutModule);
}

