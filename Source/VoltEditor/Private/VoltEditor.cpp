//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "VoltEditor.h"

#include "AssetToolsModule.h"
#include "EdGraphUtilities.h"
#include "LevelEditor.h"
#include "PropertyEditorModule.h"
#include "VoltEditorStyle.h"
#include "Asset/VoltDetailCustomizations.h"


#include "VoltAnimation.h"
#include "VoltModuleItem.h"
#include "Modules/ModuleManager.h"

#include "Widgets/Docking/SDockTab.h"


#define LOCTEXT_NAMESPACE "VoltEditorModule"


void FVoltEditorModule::StartupModule()
{
	//UThumbnailManager::Get().UnregisterCustomRenderer(UDialogueManager::StaticClass());
	//UThumbnailManager::Get().RegisterCustomRenderer(UDialogueManager::StaticClass(), UDialogueManagerThumbnail::StaticClass());

	//FDialogueEditorCommands::Register();

	RegisterModuleCommand();

	FVoltEditorStyle::ResetToDefault();

	RegisterAssetTools();
	RegisterMenuExtensions();
	RegisterToolbarExtension();

	UnregisterClassLayout(); // Remove Default Layout;
	RegisterClassLayout();
}

void FVoltEditorModule::ShutdownModule()
{
	//UThumbnailManager::Get().UnregisterCustomRenderer(UDialogueManager::StaticClass());

	UnregisterAssetTools();
	UnregisterMenuExtensions();

	UnregisterClassLayout();
}

void FVoltEditorModule::RegisterAssetTools()
{
	//IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	//EAssetTypeCategories::Type AssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Better Text")), FText::FromName(TEXT("Better Text")));

	//RegisterAssetTypeAction(AssetTools, MakeShareable(new FBetterTextAnimationActions(AssetCategory)));
	//RegisterAssetTypeAction(AssetTools, MakeShareable(new FBetterTextAnimationSetActions(AssetCategory)));
}

void FVoltEditorModule::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	RegisteredAssetTypeActions.Add(Action);
}

void FVoltEditorModule::RegisterModuleCommand()
{
	/*
	FDialogueEditorCommands::Get().PluginCommands->MapAction(
		FDialogueEditorCommands::Get().OpenDialogueManagementTab
		, FExecuteAction::CreateRaw(this, &FVoltEditorModule::OpenDialogueManagementTab)
	);
	*/
}

void FVoltEditorModule::RegisterToolbarExtension()
{
	/*
	TSharedPtr<FExtender> MyExtender = MakeShareable(new FExtender);
	MyExtender->AddToolBarExtension(
		"Game"
		, EExtensionHook::After
		, FDialogueEditorCommands::Get().PluginCommands
		, FToolBarExtensionDelegate::CreateRaw(this, &FVoltEditorModule::FillDialogueToolbar));


	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(MyExtender);
	*/
}

void FVoltEditorModule::UnregisterAssetTools()
{
	FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools");

	if (AssetToolsModule != nullptr)
	{
		IAssetTools& AssetTools = AssetToolsModule->Get();

		for (auto Action : RegisteredAssetTypeActions) { AssetTools.UnregisterAssetTypeActions(Action); }
	}
}


void FVoltEditorModule::RegisterClassLayout()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	//Register detail customizations

	PropertyModule.RegisterCustomClassLayout(FName(*UVoltModuleItem::StaticClass()->GetName())
	                                         , FOnGetDetailCustomizationInstance::CreateStatic(
		                                         &FVoltModuleDetailCustomization::MakeInstance));

	PropertyModule.RegisterCustomClassLayout(FName(*UVoltAnimation::StaticClass()->GetName())
	                                         , FOnGetDetailCustomizationInstance::CreateStatic(
		                                         &FVoltAnimationDetailCustomization::MakeInstance));
}

void FVoltEditorModule::UnregisterClassLayout()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	//Unregister detail customizations

	PropertyModule.UnregisterCustomClassLayout(FName(*UVoltModuleItem::StaticClass()->GetName()));
	PropertyModule.UnregisterCustomClassLayout(FName(*UVoltAnimation::StaticClass()->GetName()));

}

void FVoltEditorModule::RegisterMenuExtensions()
{
	MenuExtensibilityManager = MakeShareable(new FExtensibilityManager);
	ToolBarExtensibilityManager = MakeShareable(new FExtensibilityManager);
}

void FVoltEditorModule::UnregisterMenuExtensions()
{
	MenuExtensibilityManager.Reset();
	ToolBarExtensibilityManager.Reset();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FVoltEditorModule, VoltEditor)
