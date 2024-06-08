//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "Volt_ASM_InterpColor.h"
#include "VoltInterface.h"
#include "VoltVariableCollection.h"
#include "Variables/VoltVariables.h"


void UVolt_ASM_InterpColor::ModifySlateVariable(const float DeltaTime,
                                                                const TScriptInterface<IVoltInterface>& Volt)
{
	if(Volt == nullptr) return;
	if(Volt->GetVoltVariableCollection() == nullptr) return;

	UVoltVariableBase* Var = Volt->GetVoltVariableCollection()->FindOrAddVariable(UVoltVar_ColorAndOpacity::StaticClass());

	UVoltVar_ColorAndOpacity* CastedVar = Cast<UVoltVar_ColorAndOpacity>(Var);
	
	if(bUseStartColor && !bEverUpdated)
	{
		CastedVar->Value = StartColor;
	}
	
	if(InterpSpeed > 0)
	{
		CastedVar->Value.R = FMath::FInterpTo(CastedVar->Value.R, TargetColor.R, DeltaTime ,InterpSpeed);
		CastedVar->Value.G = FMath::FInterpTo(CastedVar->Value.G, TargetColor.G, DeltaTime ,InterpSpeed);
		CastedVar->Value.B = FMath::FInterpTo(CastedVar->Value.B, TargetColor.B, DeltaTime ,InterpSpeed);
		CastedVar->Value.A = FMath::FInterpTo(CastedVar->Value.A, TargetColor.A, DeltaTime ,InterpSpeed);
	}else
	{
		CastedVar->Value = TargetColor;
	}
	
	bEverUpdated = true;

}

bool UVolt_ASM_InterpColor::IsActive()
{
	if(!bEverUpdated) return true;

	const TScriptInterface<IVoltInterface>& SlateInterface = GetVoltSlate();
	if(SlateInterface == nullptr) return false;
	if(SlateInterface->GetVoltVariableCollection() == nullptr) return false;
	
	UVoltVariableBase* Var = SlateInterface->GetVoltVariableCollection()->FindOrAddVariable(UVoltVar_ColorAndOpacity::StaticClass());

	UVoltVar_ColorAndOpacity* CastedVar = Cast<UVoltVar_ColorAndOpacity>(Var);

	if(CastedVar) return CastedVar->Value != TargetColor;

	return false;
}
