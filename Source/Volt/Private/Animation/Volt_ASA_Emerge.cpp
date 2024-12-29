//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "Volt_ASA_Emerge.h"

#include "Volt_ASM_InterpRenderOpacity.h"

#include "Volt_ASM_InterpChildSlotPadding.h"
#include "Volt_ASM_SetWidgetTransformPivot.h"
#include "Module/Volt_ASM_InterpWidgetTransform.h"
#include "Slate/WidgetTransform.h"

UVolt_ASA_Emerge::UVolt_ASA_Emerge(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
	UVolt_ASM_InterpWidgetTransform* Module = ObjectInitializer.CreateDefaultSubobject<UVolt_ASM_InterpWidgetTransform>(this,"RenderTFInterp");
	Modules.Add(Module);
	Module->RateBasedInterpSpeed = 7;
	Module->bUseStartWidgetTransform = true;
	Module->StartWidgetTransform = FWidgetTransform(
		FVector2D::ZeroVector,
		FVector2D(0.5, 0.5),
		FVector2D::ZeroVector,
		0);
	Module->TargetWidgetTransform = FWidgetTransform(
		FVector2D::ZeroVector,
		FVector2D(1,1),
		FVector2D::ZeroVector,
		0);
	
	
	UVolt_ASM_InterpRenderOpacity* SlotModule = ObjectInitializer.CreateDefaultSubobject<UVolt_ASM_InterpRenderOpacity>(this,"OpacityInterp");
	Modules.Add(SlotModule);
	SlotModule->bUseStartOpacity = true;
	SlotModule->StartOpacity = 0;
	SlotModule->RateBasedInterpSpeed = 7;
	SlotModule->TargetOpacity = 1;

	UVolt_ASM_SetWidgetTransformPivot* PivotModule = ObjectInitializer.CreateDefaultSubobject<UVolt_ASM_SetWidgetTransformPivot>(this,"PivotSet");
	Modules.Add(PivotModule);
	PivotModule->TargetWidgetTransformPivot = FVector2D(0.5f);

	

}

