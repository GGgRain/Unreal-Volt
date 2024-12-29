//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "Volt_ASA_Blink.h"

#include "Volt_ASM_InterpRenderOpacity.h"
#include "Volt_ASM_Sequence.h"

UVolt_ASA_Blink::UVolt_ASA_Blink(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UVolt_ASM_Sequence* SequenceModule = ObjectInitializer.CreateDefaultSubobject<UVolt_ASM_Sequence>(this,"Sequence");
	Modules.Add(SequenceModule);
	
	UVolt_ASM_InterpRenderOpacity* ColorInModule = ObjectInitializer.CreateDefaultSubobject<UVolt_ASM_InterpRenderOpacity>(this,"ColorInterpIn");
	ColorInModule->RateBasedInterpSpeed = 7;

	UVolt_ASM_InterpRenderOpacity* ColorOutModule = ObjectInitializer.CreateDefaultSubobject<UVolt_ASM_InterpRenderOpacity>(this,"ColorInterpOut");
	ColorOutModule->RateBasedInterpSpeed = 7;

	SequenceModule->Modules.Add(ColorInModule);
	SequenceModule->Modules.Add(ColorOutModule);

}

