//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "Volt_ASA_Shrink.h"

#include "Volt_ASM_InterpChildSlotPadding.h"
#include "Layout/Margin.h"
#include "Module/Volt_ASM_InterpWidgetTransform.h"
#include "Slate/WidgetTransform.h"

class UVolt_ASM_InterpSlotPadding;

UVolt_ASA_Shrink::UVolt_ASA_Shrink(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
	UVolt_ASM_InterpWidgetTransform* SlotModule = ObjectInitializer.CreateDefaultSubobject<UVolt_ASM_InterpWidgetTransform>(this,"InterpWidgetTransform");
	Modules.Add(SlotModule);
	SlotModule->RateBasedInterpSpeed = 10;
	SlotModule->TargetWidgetTransform =  FWidgetTransform(
		FVector2D::ZeroVector,
		FVector2D(1,1),
		FVector2D::ZeroVector,
		0);
}
