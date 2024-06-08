// Fill out your copyright notice in the Description page of Project Settings.


#include "VoltSettings.h"

#include "VariableActions/VoltVariableActions.h"
#include "Variables/VoltVariables.h"

#define MACRO_REGISTER_VARIABLE_ACTION(VariableClass,VariableActionClass) \
{ \
	{ \
		if(!VariableActionsForVariable.Contains(VariableClass)) VariableActionsForVariable.Add(VariableClass,FVoltVariableActionList());\
		FVoltVariableActionList& List = VariableActionsForVariable[VariableClass];\
		List.Actions.Add(VariableActionClass);\
	} \
}

UVoltSettings::UVoltSettings()
{
	
#if WITH_EDITOR
	
	MACRO_REGISTER_VARIABLE_ACTION(UVoltVar_Opacity::StaticClass(),UVoltVarAction_Opacity::StaticClass())
	MACRO_REGISTER_VARIABLE_ACTION(UVoltVar_WidgetTransform::StaticClass(),UVoltVarAction_WidgetTransform::StaticClass())
	MACRO_REGISTER_VARIABLE_ACTION(UVoltVar_WidgetTransformPivot::StaticClass(),UVoltVarAction_WidgetTransformPivot::StaticClass())
	MACRO_REGISTER_VARIABLE_ACTION(UVoltVar_ColorAndOpacity::StaticClass(),UVoltVarAction_ColorAndOpacity::StaticClass())
	MACRO_REGISTER_VARIABLE_ACTION(UVoltVar_BackgroundColor::StaticClass(),UVoltVarAction_BackgroundColor::StaticClass())
	MACRO_REGISTER_VARIABLE_ACTION(UVoltVar_ChildSlotPadding::StaticClass(),UVoltVarAction_ChildSlotPadding::StaticClass())
	MACRO_REGISTER_VARIABLE_ACTION(UVoltVar_ParentSlotPadding::StaticClass(),UVoltVarAction_ParentSlotPadding::StaticClass())

	SaveConfig(CPF_Config, *GetDefaultConfigFilename());
	
#endif
	

}

#undef MACRO_REGISTER_VARIABLE_ACTION