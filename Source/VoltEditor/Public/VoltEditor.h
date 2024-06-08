//Copyright 2022~2024 DevGrain. All Rights Reserved.


#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Modules/ModuleInterface.h"


class IAssetTypeActions;
class IAssetTools;

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

	virtual bool SupportsDynamicReloading() override
	{
		return true;
	}

protected:

	/** Registers asset tool actions. */
	void RegisterAssetTools();

	/**
	 * Registers a single asset type action.
	 *
	 * @param AssetTools The asset tools object to register with.
	 * @param Action The asset type action to register.
	 */
	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action);

	void RegisterModuleCommand();

	void RegisterToolbarExtension();

private:
	/** Unregisters asset tool actions. */
	void UnregisterAssetTools();

	void RegisterClassLayout();


	/** Unregisters asset tool actions. */
	void UnregisterClassLayout();


protected:

	/** Registers main menu and tool bar menu extensions. */
	void RegisterMenuExtensions();

	/** Unregisters main menu and tool bar menu extensions. */
	void UnregisterMenuExtensions();

private:

	/** Holds the menu extensibility manager. */
	TSharedPtr<FExtensibilityManager> MenuExtensibilityManager;

	/** The collection of registered asset type actions. */
	TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetTypeActions;

	/** Holds the tool bar extensibility manager. */
	TSharedPtr<FExtensibilityManager> ToolBarExtensibilityManager;


public:

	

};
