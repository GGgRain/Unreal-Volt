//Copyright 2022~2024 DevGrain. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Modules/ModuleInterface.h"

namespace VoltToolTabNames
{
	static const FName VoltStarshipTab("VoltStarship");
}

class FVoltEditorModule : 
	public IHasMenuExtensibility, 
	public IHasToolBarExtensibility, 
	public IModuleInterface
{
public:
	
	//~ IHasMenuExtensibility interface

	virtual TSharedPtr<FExtensibilityManager> GetMenuExtensibilityManager() override
	{
		return MenuExtensibilityManager;
	}

public:

	//~ IHasToolBarExtensibility interface

	virtual TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager() override
	{
		return ToolBarExtensibilityManager;
	}

public:

	//~ IModuleInterface interface
	
	virtual void StartupModule() override;
	
	virtual void ShutdownModule() override;

	virtual bool SupportsDynamicReloading() override;

protected:
	
	void RegisterModuleCommand();

protected:
	
	void RegisterMajorTabs();
	
	void UnregisterMajorTabs();

public:
	
	void OpenVoltStarshipTab();
	
	TSharedRef<SDockTab> OnSpawnVoltStarshipTab(const FSpawnTabArgs& SpawnTabArgs);

protected:

	/** Registers main menu and tool bar menu extensions. */
	void RegisterMenuExtensions();

	/** Unregisters main menu and tool bar menu extensions. */
	void UnregisterMenuExtensions();

private:

	/** Holds the menu extensibility manager. */
	TSharedPtr<FExtensibilityManager> MenuExtensibilityManager;

	/** Holds the tool bar extensibility manager. */
	TSharedPtr<FExtensibilityManager> ToolBarExtensibilityManager;

};

