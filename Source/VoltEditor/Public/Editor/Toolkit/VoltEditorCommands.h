//Copyright 2022~2024 DevGrain. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

class FVoltEditorModule;
class FUICommandInfo;

class VOLTEDITOR_API FVoltEditorCommands : public TCommands<FVoltEditorCommands>
{
public:
	
	FVoltEditorCommands();

public:
	
	virtual void RegisterCommands() override;

public:
	
	TSharedPtr<FUICommandInfo> OpenVoltStarshipTab;
	
private:

	//The list of the commands that volt editor uses.
	TSharedPtr<FUICommandList> PluginCommands;

	friend FVoltEditorModule;
};
