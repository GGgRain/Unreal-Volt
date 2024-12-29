//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "Volt_ASA_Expand.h"

#include "Module/Volt_ASM_InterpWidgetTransform.h"
#include "Slate/WidgetTransform.h"

UVolt_ASA_Expand::UVolt_ASA_Expand(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UVolt_ASM_InterpWidgetTransform* SlotModule = ObjectInitializer.CreateDefaultSubobject<
		UVolt_ASM_InterpWidgetTransform>(this, "InterpWidgetTransform");
	Modules.Add(SlotModule);
	SlotModule->RateBasedInterpSpeed = 10;
	SlotModule->TargetWidgetTransform = FWidgetTransform(
		FVector2D::ZeroVector,
		FVector2D(1.02, 1.02),
		FVector2D::ZeroVector,
		0);
	
}
