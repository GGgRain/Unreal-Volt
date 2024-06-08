//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "Module/Volt_ASM_InterpWidgetTransform.h"

#include "VoltInterface.h"
#include "VoltVariableCollection.h"
#include "Variables/VoltVariables.h"


void UVolt_ASM_InterpWidgetTransform::ModifySlateVariable(const float DeltaTime,
                                                                const TScriptInterface<IVoltInterface>& Volt)
{
	if(Volt == nullptr) return;
	if(Volt->GetVoltVariableCollection() == nullptr) return;

	UVoltVariableBase* Var = Volt->GetVoltVariableCollection()->FindOrAddVariable(UVoltVar_WidgetTransform::StaticClass());

	UVoltVar_WidgetTransform* CastedVar = Cast<UVoltVar_WidgetTransform>(Var);

	if(!bEverUpdated && bUseStartWidgetTransform)
	{
		CastedVar->Value = StartWidgetTransform;
	}

	if(InterpSpeed > 0 )
	{
		CastedVar->Value.Angle = FMath::FInterpTo(CastedVar->Value.Angle,TargetWidgetTransform.Angle,DeltaTime,InterpSpeed);
		CastedVar->Value.Scale = FMath::Vector2DInterpTo(CastedVar->Value.Scale,TargetWidgetTransform.Scale,DeltaTime,InterpSpeed);
		CastedVar->Value.Shear = FMath::Vector2DInterpTo(CastedVar->Value.Shear,TargetWidgetTransform.Shear,DeltaTime,InterpSpeed);
		CastedVar->Value.Translation = FMath::Vector2DInterpTo(CastedVar->Value.Translation,TargetWidgetTransform.Translation,DeltaTime,InterpSpeed);
	}else
	{
		CastedVar->Value = TargetWidgetTransform;
	}
	bEverUpdated = true;
}

bool UVolt_ASM_InterpWidgetTransform::IsActive()
{

	if(!bEverUpdated) return true;
	
	const TScriptInterface<IVoltInterface>& SlateInterface = GetVoltSlate();
	if(SlateInterface == nullptr) return false;
	if(SlateInterface->GetVoltVariableCollection() == nullptr) return false;
	
	UVoltVariableBase* Var = SlateInterface->GetVoltVariableCollection()->FindOrAddVariable(UVoltVar_WidgetTransform::StaticClass());

	UVoltVar_WidgetTransform* CastedVar = Cast<UVoltVar_WidgetTransform>(Var);
	
	if(CastedVar) return CastedVar->Value != TargetWidgetTransform;

	return false;
	
}
