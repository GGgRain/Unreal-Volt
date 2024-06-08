//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "Volt_ASM_InterpRenderOpacity.h"

#include "VoltInterface.h"
#include "VoltVariableCollection.h"
#include "Variables/VoltVariables.h"


void UVolt_ASM_InterpRenderOpacity::ModifySlateVariable(const float DeltaTime,
                                                                const TScriptInterface<IVoltInterface>& Volt)
{
	if(Volt == nullptr) return;
	if(Volt->GetVoltVariableCollection() == nullptr) return;

	UVoltVariableBase* Var = Volt->GetVoltVariableCollection()->FindOrAddVariable(UVoltVar_Opacity::StaticClass());

	UVoltVar_Opacity* CastedVar = Cast<UVoltVar_Opacity>(Var);

	if(!bEverUpdated && bUseStartOpacity)
	{
		CastedVar->Value = StartOpacity;
	}
	
	CastedVar->Value = FMath::FInterpTo(CastedVar->Value, TargetOpacity, DeltaTime ,InterpSpeed);

	bEverUpdated = true;

}

bool UVolt_ASM_InterpRenderOpacity::IsActive()
{
	if(!bEverUpdated) return true;

	const TScriptInterface<IVoltInterface>& SlateInterface = GetVoltSlate();
	if(SlateInterface == nullptr) return false;
	if(SlateInterface->GetVoltVariableCollection() == nullptr) return false;
	
	UVoltVariableBase* Var = SlateInterface->GetVoltVariableCollection()->FindOrAddVariable(UVoltVar_Opacity::StaticClass());

	UVoltVar_Opacity* CastedVar = Cast<UVoltVar_Opacity>(Var);
	
	if(CastedVar) return CastedVar->Value != TargetOpacity;

	return false;
}
