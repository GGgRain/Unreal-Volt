//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "Volt_ASA_ChangeColor_Blink.h"

#include "Volt_ASM_InterpColor.h"
#include "Volt_ASM_Sequence.h"

UVolt_ASA_ChangeColor_Blink::UVolt_ASA_ChangeColor_Blink(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UVolt_ASM_Sequence* SequenceModule = ObjectInitializer.CreateDefaultSubobject<UVolt_ASM_Sequence>(this,"Sequence");
	Modules.Add(SequenceModule);
	
	UVolt_ASM_InterpColor* ColorInModule = ObjectInitializer.CreateDefaultSubobject<UVolt_ASM_InterpColor>(this,"ColorInterpIn");
	ColorInModule->RateBasedInterpSpeed = 10;

	UVolt_ASM_InterpColor* ColorOutModule = ObjectInitializer.CreateDefaultSubobject<UVolt_ASM_InterpColor>(this,"ColorInterpOut");
	ColorOutModule->RateBasedInterpSpeed = 10;

	SequenceModule->Modules.Add(ColorInModule);
	SequenceModule->Modules.Add(ColorOutModule);
	
	
	/*
	 *
	*	UVoltAnimation* Anim = VOLT_MAKE_ANIMATION(UVoltAnimation)
	(
		VOLT_MAKE_MODULE(UVolt_ASM_InterpColor)
		.InterpSpeed(123)
		.StartColor(FLinearColor::Yellow),
		VOLT_MAKE_MODULE(UVolt_ASM_InterpColor)
		.InterpSpeed(4)
		.StartColor(FLinearColor::Blue)
	);
	
	VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
		.bShouldLoop(false)
		[
			VOLT_MAKE_MODULE(UVolt_ASM_InterpColor)
				.InterpSpeed(5)
				.StartColor(FLinearColor::Black),
			VOLT_MAKE_MODULE(UVolt_ASM_InterpColor)
				.InterpSpeed(5)
				.StartColor(FLinearColor::Black)
		],
	*/

}


