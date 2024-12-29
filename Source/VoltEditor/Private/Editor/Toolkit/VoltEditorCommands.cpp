//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "Toolkit/VoltEditorCommands.h"

#include "VoltEditorStyle.h"
#include "Framework/Commands/UICommandInfo.h"
#include "Framework/Commands/Commands.h"

#define LOCTEXT_NAMESPACE "VoltEditorCommands"


FVoltEditorCommands::FVoltEditorCommands()
	: TCommands<FVoltEditorCommands>(
		TEXT("VoltEditor"), // Context name for fast lookup
		NSLOCTEXT("Contexts", "VoltEditor", "Volt Editor"), // Localized context name for displaying
		NAME_None, // Parent
		FVoltEditorStyle::GetUEEditorSlateStyleSetName() // Icon Style Set
	)
{
}

void FVoltEditorCommands::RegisterCommands()
{
	//Initializing a new command list.
	PluginCommands = MakeShareable(new FUICommandList());

	UI_COMMAND(OpenVoltStarshipTab, "Open Volt Starship", "Open volt starship tab that involves sample animations and showcase layout", EUserInterfaceActionType::Button, FInputChord(EModifierKey::Control,EKeys::F1));
	
}

#undef LOCTEXT_NAMESPACE
