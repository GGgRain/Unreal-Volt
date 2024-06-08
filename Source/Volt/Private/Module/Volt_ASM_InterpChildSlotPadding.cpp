//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "Volt_ASM_InterpChildSlotPadding.h"

#include "VoltInterface.h"
#include "VoltVariableCollection.h"
#include "Variables/VoltVariables.h"


void UVolt_ASM_InterpChildSlotPadding::ModifySlateVariable(const float DeltaTime,
                                                                const TScriptInterface<IVoltInterface>& Volt)
{
	if(Volt == nullptr) return;
	if(Volt->GetVoltVariableCollection() == nullptr) return;

	UVoltVariableBase* Var = Volt->GetVoltVariableCollection()->FindOrAddVariable(UVoltVar_ChildSlotPadding::StaticClass());

	UVoltVar_ChildSlotPadding* CastedVar = Cast<UVoltVar_ChildSlotPadding>(Var);
	CastedVar->Value.Bottom = FMath::FInterpTo(CastedVar->Value.Bottom, TargetPadding.Bottom, DeltaTime ,InterpSpeed);
	CastedVar->Value.Top = FMath::FInterpTo(CastedVar->Value.Top, TargetPadding.Top, DeltaTime ,InterpSpeed);
	CastedVar->Value.Right = FMath::FInterpTo(CastedVar->Value.Right, TargetPadding.Right, DeltaTime ,InterpSpeed);
	CastedVar->Value.Left = FMath::FInterpTo(CastedVar->Value.Left, TargetPadding.Left, DeltaTime ,InterpSpeed);

}

bool UVolt_ASM_InterpChildSlotPadding::IsActive()
{

	const TScriptInterface<IVoltInterface>& SlateInterface = GetVoltSlate();
	if(SlateInterface == nullptr) return false;
	if(SlateInterface->GetVoltVariableCollection() == nullptr) return false;
	
	UVoltVariableBase* Var = SlateInterface->GetVoltVariableCollection()->FindOrAddVariable(UVoltVar_ChildSlotPadding::StaticClass());

	UVoltVar_ChildSlotPadding* CastedVar = Cast<UVoltVar_ChildSlotPadding>(Var);
	
	if(CastedVar) return CastedVar->Value != TargetPadding;

	return false;
}
